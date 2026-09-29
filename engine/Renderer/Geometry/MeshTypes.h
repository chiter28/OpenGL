#pragma once

#include <limits>
#include <glm/glm.hpp>

#include "Renderer/Resources/Layout.h"




using ModelVertex = Vertex<Position, Color, TexCoord, Normal>;


struct SubMesh
{
	static constexpr uint32_t InvalidMaterialIndex = std::numeric_limits<uint32_t>::max();

	uint32_t IndexOffset = 0;
	uint32_t IndexCount = 0;
	uint32_t VertexOffset = 0;
	uint32_t MaterialIndex = InvalidMaterialIndex;
};


struct MeshInstance
{
	size_t MeshIndex = 0;

	// Transforms mesh coordinates into model coordinates.
	glm::mat4 Transform = { 1.0f };
};