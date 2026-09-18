#include <utility>

#include "Buffer.h"
#include <glad/glad.h>


// VertexBuffer
VertexBuffer::VertexBuffer(const void* data, uint32_t size)
{
	glCreateBuffers(1, &m_ID);
	glNamedBufferStorage(m_ID, size, data, 0);
}



VertexBuffer::VertexBuffer(const void* data, uint32_t size, const VertexBufferLayout& layout)
	: m_Layout(layout)
{
	glCreateBuffers(1, &m_ID);
	glNamedBufferStorage(m_ID, size, data, 0);
}



VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	: m_ID(std::exchange(other.m_ID, 0)), m_Layout(std::move(other.m_Layout))
{
	other.m_Layout.Clear();
}



VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
	if (this == &other)
		return *this;

	Release();
	m_ID = std::exchange(other.m_ID, 0);
	m_Layout = std::move(other.m_Layout);

	other.m_Layout.Clear();
	
	return *this;
}



void VertexBuffer::SetLayout(const std::initializer_list<VertexAttribute>& vertexElements)
{
	VertexBufferLayout layout(vertexElements);
	m_Layout = layout;
}
	
void VertexBuffer::Release()
{
	if (m_ID != 0)
	{
		glDeleteBuffers(1, &m_ID);
		m_ID = 0;
	}

	m_Layout.Clear();
	

}

VertexBuffer::~VertexBuffer()
{
	Release();
}




// IndexBuffer
IndexBuffer::IndexBuffer(std::span<uint32_t> indexBuffer)
{
	glCreateBuffers(1, &m_ID);
	glNamedBufferStorage(m_ID, indexBuffer.size_bytes(), indexBuffer.data(), 0);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
	: m_ID(std::exchange(other.m_ID, 0))
{}

IndexBuffer& IndexBuffer::operator=(IndexBuffer && other) noexcept
{
	if (this == &other)
		return *this;

	Release();
	m_ID = std::exchange(other.m_ID, 0);
	return *this;
}



void IndexBuffer::Release()
{
	if (m_ID != 0)
	{
		glDeleteBuffers(1, &m_ID);
		m_ID = 0;
	}
}

IndexBuffer::~IndexBuffer()
{
	Release();
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
