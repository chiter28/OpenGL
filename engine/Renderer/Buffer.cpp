#include "Buffer.h"
#include <glad/glad.h>




VertexBuffer::VertexBuffer(const void* data, uint32_t size)
{
	glCreateBuffers(1, &m_VBO);
	glNamedBufferStorage(m_VBO, size, data, 0);
}

// VertexBuffer
VertexBuffer::VertexBuffer(const void* data, uint32_t size, const VertexBufferLayout& layout)
	: m_Layout(layout)
{
	glCreateBuffers(1, &m_VBO);
	glNamedBufferStorage(m_VBO, size, data, 0);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_VBO);
}

void VertexBuffer::SetLayout(const std::initializer_list<VertexAttribute>& vertexElements)
{
	VertexBufferLayout layout(vertexElements);
	m_Layout = layout;
}
	




// IndexBuffer
IndexBuffer::IndexBuffer(std::span<uint32_t> indexBuffer)
{
	glCreateBuffers(1, &m_IBO);
	glNamedBufferStorage(m_IBO, indexBuffer.size_bytes(), indexBuffer.data(), 0);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_IBO);
}





MultiVertexBuffer::MultiVertexBuffer()
{
	glCreateBuffers(static_cast<size_t>(BufferType::Count), m_Buffers);
}

MultiVertexBuffer::~MultiVertexBuffer()
{
	glDeleteBuffers(static_cast<size_t>(BufferType::Count), m_Buffers);
}

void MultiVertexBuffer::SetData(BufferType type, const void* data, uint32_t size)
{
	glNamedBufferStorage(GetID(type), size, data, 0);
}

void MultiVertexBuffer::SetLayout(BufferType type, VertexAttribute vertexAttribute)
{
	VertexBufferLayout layout({ vertexAttribute });
	m_Layouts[static_cast<size_t>(type)] = layout;
}
