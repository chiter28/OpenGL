#pragma once

#include <limits>

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
