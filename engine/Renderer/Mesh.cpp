#include "Mesh.h"

#include <glad/glad.h>

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <filesystem>


#define GetCountOfElements(buffers) { sizeof(buffers) / sizeof(buffers[0])}


// glTF 2.0 (.glb) - новый, .fbx - старый
Mesh::~Mesh()
{}

bool Mesh::LoadMesh(const std::string& fileName, const std::shared_ptr<Shader>& shader)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices);
	
	if (!scene) {
		std::cerr << "Error parsing " << fileName << ": " << importer.GetErrorString();
		return false;
	}

	InitFromScene(scene, fileName);
	InitMaterials(scene, fileName, shader);
	PopulateBuffers();

	return true;
}

bool Mesh::InitFromScene(const aiScene* scene, const std::string& fileName)
{
	// mesh
	m_SubMeshes.resize(scene->mNumMeshes);
	uint32_t NumVertices = 0;
	uint32_t NumIndices = 0;

	CountVerticesAndIndices(scene, NumVertices, NumIndices);

	m_Positions.reserve(NumVertices);
	m_TexCoords.reserve(NumVertices);
	m_Indices.reserve(NumIndices);

	InitMeshes(scene);

	return true;
}

void Mesh::CountVerticesAndIndices(const aiScene* scene, uint32_t& numVertices, uint32_t& numIndices)
{
	for (uint32_t i = 0; i < m_SubMeshes.size(); i++) {

		m_SubMeshes[i].NumIndices = scene->mMeshes[i]->mNumFaces * 3;
		m_SubMeshes[i].BaseIndex = numIndices;
		m_SubMeshes[i].BaseVertex = numVertices;
		m_SubMeshes[i].MaterialIndex = scene->mMeshes[i]->mMaterialIndex;

		numIndices += m_SubMeshes[i].NumIndices;
		numVertices += scene->mMeshes[i]->mNumVertices;
	}
}

void Mesh::InitMeshes(const aiScene* scene)
{
	for (uint32_t m = 0; m < m_SubMeshes.size(); m++)
	{
		const aiMesh* mesh = scene->mMeshes[m];
		for (int v = 0; v < mesh->mNumVertices; v++)
		{
			const aiVector3D& positions = mesh->mVertices[v];
			const aiVector3D& texCoords = mesh->HasTextureCoords(0) ? (*mesh->mTextureCoords)[v] : aiVector3D(0.0f, 0.0f, 0.0f);

			m_Positions.emplace_back(glm::vec3{ positions.x, positions.y, positions.z });
			m_TexCoords.emplace_back(glm::vec2(texCoords.x, texCoords.y));
		}

		// m_Indices
		for (int f = 0; f < mesh->mNumFaces; f++)
		{
			const aiFace& face = mesh->mFaces[f];
			assert(face.mNumIndices == 3);
			for (int i = 0; i < face.mNumIndices; i++) {
				m_Indices.push_back(face.mIndices[i]);
			}
		}
	}
}

void Mesh::InitMaterials(const aiScene* scene, const std::string& fileName, const std::shared_ptr<Shader>& defaultShader)
{
	m_Materials.resize(scene->mNumMaterials);
	
	std::filesystem::path modelDir = std::filesystem::path(fileName).parent_path();

	for (int m = 0; m < scene->mNumMaterials; m++)
	{
		const aiMaterial* material = scene->mMaterials[m];

		std::shared_ptr<Material> tempMaterial = std::make_shared<Material>(defaultShader);

		tempMaterial->SetTransparent(IsMaterialTransparent(material));

		std::shared_ptr<Texture> texture = nullptr;
		aiString path;
		if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &path) == AI_SUCCESS ||
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(path.C_Str());
			if (embeddedTexture) // Встроенная текстура (.glb)
			{
				if (embeddedTexture->mHeight == 0) // Сжатый формат (PNG/JPG)
				{
					texture = std::make_shared<Texture>(
						reinterpret_cast<uint8_t*>(embeddedTexture->pcData), embeddedTexture->mWidth);
					tempMaterial->SetTexture("uSampler", texture, 0);

				} else // Несжатые сырые RGBA пиксели
				{
					texture = std::make_shared<Texture>(
						reinterpret_cast<uint8_t*>(embeddedTexture->pcData), embeddedTexture->mWidth, embeddedTexture->mHeight);
					tempMaterial->SetTexture("uSampler", texture, 0);
				}
			} 
			else // Внешняя текстура на диске (.gltf)
			{
				std::filesystem::path fullPath = modelDir / path.C_Str();
				if (std::filesystem::exists(fullPath)) {
					texture = std::make_shared<Texture>(fullPath.string());
					tempMaterial->SetTexture("uSampler", texture, 0);
				}

			}
		}
		if (!texture)
		{
			aiColor4D color(1.0f, 1.0f, 1.0f, 1.0f);
			if (material->Get(AI_MATKEY_BASE_COLOR, color) != AI_SUCCESS) {
				material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			}

			uint8_t pixel[4] = {
				static_cast<uint8_t>(color.r * 255),
				static_cast<uint8_t>(color.g * 255),
				static_cast<uint8_t>(color.b * 255),
				static_cast<uint8_t>(color.a * 255)
			};
			texture = std::make_shared<Texture>(pixel, 1, 1);
			tempMaterial->SetTexture("uSampler", texture, 0);
		}

		aiColor3D ambientColor;
		if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS)
		{
			std::cout << "Load ambient color\n";
			tempMaterial->SetVec3("uMaterial.AmbientColor", { ambientColor.r, ambientColor.g, ambientColor.b });
		} else
		{
			tempMaterial->SetVec3("uMaterial.AmbientColor", { 1.0f, 1.0f, 1.0f });
		}

		m_Materials[m] = std::move(tempMaterial);
	}


}

bool Mesh::IsMaterialTransparent(const aiMaterial* material)
{
	float opacity = 1.0f;
	if (material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS)
	{
		if (opacity < 0.99f)
			return true;
	}

	aiColor4D baseColor;
	if (material->Get(AI_MATKEY_BASE_COLOR, baseColor) == AI_SUCCESS)
	{
		if (baseColor.a < 0.99f)
			return true;
	}

	aiColor4D diffuseColor;
	if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
	{
		if (diffuseColor.a < 0.99f)
			return true;
	}

	return false;
}

void Mesh::PopulateBuffers()
{
	m_VertexArray = std::make_shared<VertexArray>();

	std::shared_ptr<MultiVertexBuffer> m_MultiVertexBuffer = std::make_shared<MultiVertexBuffer>();

	// position
	m_MultiVertexBuffer->SetData(BufferType::Position, m_Positions.data(), m_Positions.size() * sizeof(glm::vec3));
	m_MultiVertexBuffer->SetLayout(BufferType::Position, { VertexAttribute::Position });
	m_VertexArray->AddMultiVertexBuffer(BufferType::Position, m_MultiVertexBuffer);


	// texture coords (UV)
	m_MultiVertexBuffer->SetData(BufferType::TexCoord, m_TexCoords.data(), m_TexCoords.size() * sizeof(glm::vec2));
	m_MultiVertexBuffer->SetLayout(BufferType::TexCoord, { VertexAttribute::TexCoord });
	m_VertexArray->AddMultiVertexBuffer(BufferType::TexCoord, m_MultiVertexBuffer);


	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(m_Indices);
	m_VertexArray->AddIndexBuffer(indexBuffer);

	m_Positions.clear();  m_Positions.shrink_to_fit();
	m_TexCoords.clear();  m_TexCoords.shrink_to_fit();
	m_Indices.clear();	  m_Indices.shrink_to_fit();
}

void Mesh::Render()
{
	m_VertexArray->Bind();
	
	// Opaque materials
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	for (int m = 0; m < m_SubMeshes.size(); m++) {
		uint32_t matIndex = m_SubMeshes[m].MaterialIndex;

		if (matIndex >= m_Materials.size() || m_Materials[matIndex]->IsTransparent())
			continue;

		if (m_Materials[matIndex])
			m_Materials[matIndex]->Bind();


		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_SubMeshes[m].NumIndices,
			GL_UNSIGNED_INT,
			(const void*)(uintptr_t)(m_SubMeshes[m].BaseIndex * sizeof(uint32_t)),
			m_SubMeshes[m].BaseVertex);
	}


	// Transparent materials
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int m = 0; m < m_SubMeshes.size(); m++) {
		uint32_t matIndex = m_SubMeshes[m].MaterialIndex;

		if (matIndex >= m_Materials.size() || !m_Materials[matIndex]->IsTransparent())
			continue;

		if (m_Materials[matIndex])
			m_Materials[matIndex]->Bind();

		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_SubMeshes[m].NumIndices,
			GL_UNSIGNED_INT,
			(const void*)(uintptr_t)(m_SubMeshes[m].BaseIndex * sizeof(uint32_t)),
			m_SubMeshes[m].BaseVertex);
	}
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}


