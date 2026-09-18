#pragma once

#include <memory>
#include <vector>
#include <limits>

#include <glm/glm.hpp>

#include "Renderer/Resources/VertexArray.h"

#include "MeshTypes.h"


enum class ModelLoaderBackend
{
	Auto,
	Assimp,
	FastGltf
};






class Mesh
{
public:

	Mesh(Mesh&&) noexcept = default;
	Mesh& operator=(Mesh&&) noexcept = default;

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	~Mesh() = default;


	// main constructor
	Mesh(std::vector<ModelVertex> vertices, std::vector<uint32_t> indices, std::vector<SubMesh> subMeshes);
	

	const VertexArray& GetVertexArray() const noexcept { return *m_VertexArray; }
	const std::vector<SubMesh>& GetSubMeshes() const noexcept { return  m_SubMeshes; }


private:
	std::unique_ptr<VertexArray> m_VertexArray;
	std::vector<SubMesh> m_SubMeshes;
};