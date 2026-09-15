#pragma once

#include "VertexArray.h"
#include "Texture.h"
#include "Material.h"

#include <assimp/scene.h>

#include <string>
#include <memory>
#include <vector>




class Mesh
{
public:
	struct SubMeshElement
	{
		uint32_t NumIndices = 0;
		uint32_t BaseIndex = 0;
		uint32_t BaseVertex = 0;
		uint32_t MaterialIndex = 0xFFFFFFFF;
	};

public:
	Mesh() = default;
	~Mesh();

	bool LoadMesh(const std::string& fileName, const std::shared_ptr<Shader>& shader);


	const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }
	const std::vector<SubMeshElement>& GetSubMeshes() const { return m_SubMeshes; }
	std::shared_ptr<Material> GetMaterial(uint32_t index) const
	{
		return (index < m_Materials.size()) ? m_Materials[index] : nullptr;
	}


private:
	bool InitFromScene(const aiScene* scene, const std::string& fileName);
	void CountVerticesAndIndices(const aiScene* scene, uint32_t& numVertices, uint32_t& numIndices);
	void InitMeshes(const aiScene* scene);
	void InitMaterials(const aiScene* scene, const std::string& fileName, const std::shared_ptr<Shader>& defaultShader);
	bool IsMaterialTransparent(const aiMaterial* material);
	void PopulateBuffers();



private:
	std::vector<SubMeshElement> m_SubMeshes;
	std::vector<std::shared_ptr<Material>> m_Materials;

	std::shared_ptr<VertexArray> m_VertexArray;
	std::vector<Vertex<Position, TexCoord, Normal>> m_Vertices;
	std::vector<uint32_t> m_Indices;
};