#pragma once

#include <memory>
#include <vector>
#include <limits>

#include <glm/glm.hpp>

#include "VertexArray.h"



enum class ModelLoaderBackend
{
	Auto,
	Assimp,
	FastGltf
};




struct SubMesh
{
	static constexpr uint32_t InvalidMaterialIndex = std::numeric_limits<uint32_t>::max();
	
	uint32_t IndexOffset = 0;
	uint32_t IndexCount = 0;
	uint32_t VertexOffset = 0;
	uint32_t MaterialIndex = InvalidMaterialIndex;
};


class Mesh
{
public:
	using VertexType = Vertex<Position, Color, TexCoord, Normal>;

	Mesh(Mesh&&) noexcept = default;
	Mesh& operator=(Mesh&&) noexcept = default;

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	~Mesh() = default;


	// main constructor
	Mesh(std::vector<VertexType> vertices, std::vector<uint32_t> indices, std::vector<SubMesh> subMeshes);
	

	const VertexArray&           GetVertexArray()  const noexcept { return *m_VertexArray; }
	const std::vector<SubMesh>& GetSubMeshes()   const noexcept { return  m_SubMeshes;   }


private:
	std::unique_ptr<VertexArray> m_VertexArray;
	std::vector<SubMesh> m_SubMeshes;
};