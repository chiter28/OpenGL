#pragma once

#include "Renderer/Models/ModelData.h"

namespace Primitives
{
	MeshData CreateCubeMesh(float size);
	MeshData CreateSphereMesh(float radius, uint32_t sectors, uint32_t stacks);
}