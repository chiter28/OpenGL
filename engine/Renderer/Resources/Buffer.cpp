#include <utility>

#include "Buffer.h"
#include <glad/glad.h>
#include <stdexcept>


// VertexBuffer
VertexBuffer::VertexBuffer(const void* data, uint32_t size)
{
	glCreateBuffers(1, &m_ID);
	glNamedBufferStorage(m_ID, size, data, 0);
}

void VertexBuffer::SetLayout(const std::initializer_list<VertexAttribute>& vertexElements)
{
	m_Layout = VertexBufferLayout{ vertexElements };
}

void VertexBuffer::SetDebugName(const char* name) const
{
	if (m_ID != 0)
	{
		glObjectLabel(GL_BUFFER, m_ID, -1, name);
	}
}





VertexBuffer::VertexBuffer(const void* data, uint32_t size, VertexBufferLayout layout)
	: m_Layout(std::move(layout))
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

void IndexBuffer::SetDebugName(const char* name) const
{
	if (m_ID != 0)
	{
		glObjectLabel(GL_BUFFER, m_ID, -1, name);
	}
}

IndexBuffer::~IndexBuffer()
{
	Release();
}





// UniformBuffer

UniformBuffer::UniformBuffer(uint32_t size)
	: m_Size(size)
{
	if (m_Size == 0)
	{
		throw std::invalid_argument("UniformBuffer size must be positive");
	}
	glCreateBuffers(1, &m_ID);
	glNamedBufferStorage(m_ID, static_cast<GLsizeiptr>(m_Size), nullptr, GL_DYNAMIC_STORAGE_BIT);
}

UniformBuffer::~UniformBuffer()
{
	if (m_ID != 0)
	{
		glDeleteBuffers(1, &m_ID);
	}
}

void UniformBuffer::SetData(std::span<const std::byte> data) const
{
	if (data.size_bytes() != m_Size)
	{
		throw std::invalid_argument("UniformBuffer data size must match buffer size");
	}

	glNamedBufferSubData(m_ID, 0, data.size_bytes(), data.data());
}


void UniformBuffer::Bind(uint32_t bindingPoint) const
{
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_ID);
}

void UniformBuffer::SetDebugName(const char* name) const
{
	if (m_ID != 0)
	{
		glObjectLabel(GL_BUFFER, m_ID, -1, name);
	}
}
