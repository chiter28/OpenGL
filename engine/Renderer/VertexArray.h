#pragma once

#include <vector>
#include <memory>
#include "Buffer.h"


class VertexArray
{
public:
	VertexArray();
	VertexArray(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer);
	~VertexArray();

	void Bind() const;

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, uint32_t divisor = 0);
	void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

private:
	uint32_t m_VAO;
	std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	uint32_t m_AttributeIndex = 0;
	uint32_t m_VertexBindingIndex = 0;
};
