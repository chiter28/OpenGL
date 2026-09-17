#include "Mesh2.h"


Mesh2::Mesh2(std::vector<VertexType> vertices, std::vector<uint32_t> indices, std::vector<SubMesh2> subMeshes)
	: m_SubMeshes(std::move(subMeshes))
{
	std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices);
	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices);

	m_VertexArray = std::make_unique<VertexArray>(vertexBuffer, indexBuffer);
}
