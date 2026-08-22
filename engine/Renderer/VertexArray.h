#pragma once

#include <vector>
#include <memory>
#include "Buffer.h"


class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
	void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

private:
	uint32_t m_VertexBindingIndex = 0;
	uint32_t m_VAO;
	std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	uint32_t m_AttributeIndex = 0;
};
