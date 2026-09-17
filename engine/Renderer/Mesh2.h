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




struct SubMesh2
{
	static constexpr uint32_t InvalidMaterialIndex = std::numeric_limits<uint32_t>::max();
	
	uint32_t indexOffset = 0;
	uint32_t indexCount = 0;
	uint32_t vertexOffset = 0;
	uint32_t materialIndex = InvalidMaterialIndex;
};


class Mesh2
{
public:
	using VertexType = Vertex<Position, Color, TexCoord, Normal>;

	Mesh2(Mesh2&&) noexcept = default;
	Mesh2& operator=(Mesh2&&) noexcept = default;

	Mesh2(const Mesh2&) = delete;
	Mesh2& operator=(const Mesh2&) = delete;

	Mesh2(std::vector<VertexType> vertices, std::vector<uint32_t> indices, std::vector<SubMesh2> subMesh2es);
	~Mesh2() = default;
	
	const VertexArray&           GetVertexArray()  const noexcept { return *m_VertexArray; }
	const std::vector<SubMesh2>& GetSubMesh2es()   const noexcept { return  m_SubMeshes;   }


private:
	std::unique_ptr<VertexArray> m_VertexArray;
	std::vector<SubMesh2> m_SubMeshes;
};