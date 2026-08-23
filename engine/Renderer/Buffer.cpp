#include "Buffer.h"
#include <glad/glad.h>

// VertexBuffer
VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
{
	glCreateBuffers(1, &m_VBO);
	glNamedBufferStorage(m_VBO, vertices.size() * sizeof(Vertex), vertices.data(), 0);
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

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}





VertexBufferLayout::VertexBufferLayout(const std::initializer_list<VertexAttribute>& vertexElements)
{
	for (auto& element : vertexElements) {

		BufferElement bufferElement;

		switch (element)
		{
			case VertexAttribute::Position:
			{
				bufferElement.Attribute = VertexAttribute::Position;
				bufferElement.Type = ShaderDataType::Float3;
				bufferElement.Size = GetShaderDataTypeSize(ShaderDataType::Float3);
				bufferElement.Offset = m_Stride;
				m_Stride += bufferElement.Size;
				bufferElement.Normalized = false;
				break;
			}

			case VertexAttribute::Color:
			{
				bufferElement.Attribute = VertexAttribute::Color;
				bufferElement.Type = ShaderDataType::Float3;
				bufferElement.Size = GetShaderDataTypeSize(ShaderDataType::Float3);
				bufferElement.Offset = m_Stride;
				m_Stride += bufferElement.Size;
				bufferElement.Normalized = false;
				break;
			}

			case VertexAttribute::TexCoord:
			{
				bufferElement.Attribute = VertexAttribute::TexCoord;
				bufferElement.Type = ShaderDataType::Float2;
				bufferElement.Size = GetShaderDataTypeSize(ShaderDataType::Float2);
				bufferElement.Offset = m_Stride;
				m_Stride += bufferElement.Size;
				bufferElement.Normalized = false;
				break;
			}
		}
		m_BufferElements.emplace_back(bufferElement);
	}

}

uint32_t VertexBufferLayout::BufferElement::GetComponentCount() const
{
	switch (Type)
	{
		case ShaderDataType::Float:    return 1;
		case ShaderDataType::Float2:   return 2;
		case ShaderDataType::Float3:   return 3;
		case ShaderDataType::Float4:   return 4;
		case ShaderDataType::Int:      return 1;
		case ShaderDataType::Int2:     return 2;
		case ShaderDataType::Int3:     return 3;
		case ShaderDataType::Int4:     return 4;
		case ShaderDataType::Mat3:     return 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		default:
			return 0;
	}
}

uint32_t VertexBufferLayout::GetShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Bool:     return 1;
		default:
			return 0;
	}
}

uint32_t VertexBufferLayout::ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Bool:     return GL_BOOL;
		default:
			return 0;
	}
}
