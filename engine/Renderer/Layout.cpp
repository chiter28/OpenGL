#include "Layout.h"
#include <glad/glad.h>

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
			case VertexAttribute::Instance:
			{
				bufferElement.Attribute = VertexAttribute::Instance;
				bufferElement.Type = ShaderDataType::Float3;
				bufferElement.Size = GetShaderDataTypeSize(ShaderDataType::Float3);
				bufferElement.Offset = m_Stride;
				m_Stride += bufferElement.Size;
				bufferElement.Normalized = false;
				break;
			}
			case VertexAttribute::Normal:
			{
				bufferElement.Attribute = VertexAttribute::Normal;
				bufferElement.Type = ShaderDataType::Float3;
				bufferElement.Size = GetShaderDataTypeSize(ShaderDataType::Float3);
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
