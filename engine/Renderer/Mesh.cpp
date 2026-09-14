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

	m_Vertices.reserve(NumVertices);
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
			const aiVector3D& texCoords = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][v] : aiVector3D(0.0f, 0.0f, 0.0f);
			const aiVector3D& normals = mesh->mNormals[v];
			
			m_Vertices.emplace_back(
				Position{{ positions.x, positions.y, positions.z }},
				TexCoord{{texCoords.x, texCoords.y}},
				Normal{{normals.x, normals.y, normals.z}}
			);
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
	m_Materials.reserve(scene->mNumMaterials);
	std::filesystem::path modelDir = std::filesystem::path(fileName).parent_path();

	for (int m = 0; m < scene->mNumMaterials; m++)
	{
		const aiMaterial* material = scene->mMaterials[m];

		m_Materials.emplace_back(std::make_shared<Material>(defaultShader));
		m_Materials[m]->SetTransparent(IsMaterialTransparent(material));

		std::shared_ptr<Texture> texture = nullptr;
		aiString path;

		if (material->GetTexture(AI_MATKEY_BASE_COLOR_TEXTURE, &path) == AI_SUCCESS ||
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(path.C_Str());
			if (embeddedTexture) // Встроенная текстура (.glb)
			{
				if (embeddedTexture->mHeight == 0) // Сжатый формат (PNG/JPG)
				{
					texture = std::make_shared<Texture>(
						reinterpret_cast<uint8_t*>(embeddedTexture->pcData), embeddedTexture->mWidth);
				} else // Несжатые сырые RGBA пиксели
				{
					texture = std::make_shared<Texture>(
						reinterpret_cast<uint8_t*>(embeddedTexture->pcData), embeddedTexture->mWidth, embeddedTexture->mHeight);
				}
			} else // Внешняя текстура на диске (.gltf)
			{
				std::filesystem::path fullPath = modelDir / path.C_Str();
				if (std::filesystem::exists(fullPath)) {
					texture = std::make_shared<Texture>(fullPath.string());
				}
			}
		}

		aiColor3D specularColor(0.0f, 0.0f, 0.0f);
		if (material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS)
		{
			std::cout << "Specular";
		}


		if (texture)
		{
			m_Materials[m]->SetTexture("u_AlbedoMap", texture, 0);
			m_Materials[m]->SetVec4("u_Material.BaseColorFactor", glm::vec4(1.0f));
		}
		else
		{
			aiColor4D baseColor(1.0f, 1.0f, 1.0f, 1.0f);
			if (material->Get(AI_MATKEY_BASE_COLOR, baseColor) != AI_SUCCESS) {
				material->Get(AI_MATKEY_COLOR_DIFFUSE, baseColor);
			}

			uint32_t whiteTextureData = 0xffffffff;
			texture = std::make_shared<Texture>(&whiteTextureData, 1, 1); // white texture

			m_Materials[m]->SetTexture("u_AlbedoMap", texture, 0);
			m_Materials[m]->SetVec4("u_Material.BaseColorFactor",
				glm::vec4(baseColor.r, baseColor.g, baseColor.b, baseColor.a));
		}
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
	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(m_Vertices);
	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(m_Indices);
	
	m_VertexArray = std::make_shared<VertexArray>(vertexBuffer, indexBuffer);

	m_Indices.clear();
	m_Indices.shrink_to_fit();
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


