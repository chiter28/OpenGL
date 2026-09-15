#include "VertexArray.h"
#include "../Utils.h"
#include <glad/glad.h>

static uint32_t GetAttributeLocation(VertexAttribute attrib)
{
	switch (attrib)
	{
		case VertexAttribute::Position:	  return 0;
		case VertexAttribute::Color:	  return 1;
		case VertexAttribute::TexCoord:	  return 2;
		case VertexAttribute::Instance:	  return 3;
		case VertexAttribute::Normal:	  return 4;
		default: return 999;
	}
}


VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_VAO);
}

VertexArray::VertexArray(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	glCreateVertexArrays(1, &m_VAO);
	AddVertexBuffer(vertexBuffer);
	AddIndexBuffer(indexBuffer);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_VAO);
}

void VertexArray::AddMultiVertexBuffer(BufferType type, const std::shared_ptr<MultiVertexBuffer>& multiVertexBuffer)
{
	glVertexArrayVertexBuffer(m_VAO, m_VertexBindingIndex, multiVertexBuffer->GetID(type), 0, multiVertexBuffer->GetLayout(type).GetStride());

	for (const VertexBufferLayout::BufferElement& element : multiVertexBuffer->GetLayout(type).GetElements())
	{
		uint32_t attributeLocation = GetAttributeLocation(element.Attribute);

		glEnableVertexArrayAttrib(m_VAO, attributeLocation);
		glVertexArrayAttribFormat(
			m_VAO,
			attributeLocation,
			element.GetComponentCount(),
			VertexBufferLayout::ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized,
			element.Offset
		);

		glVertexArrayAttribBinding(m_VAO, attributeLocation, m_VertexBindingIndex);
	}

	if (m_MultiVertexBuffer != multiVertexBuffer)
	{
		m_MultiVertexBuffer = multiVertexBuffer;
	}

	m_VertexBindingIndex++;
}


void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, uint32_t divisor)
{
	glVertexArrayVertexBuffer(m_VAO, m_VertexBindingIndex, vertexBuffer->GetID(), 0, vertexBuffer->GetLayout().GetStride());

	if (divisor > 0)
		glVertexArrayBindingDivisor(m_VAO, m_VertexBindingIndex, divisor);

	for (const VertexBufferLayout::BufferElement& element : vertexBuffer->GetLayout().GetElements())
	{
		uint32_t attributeLocation = GetAttributeLocation(element.Attribute);

		glEnableVertexArrayAttrib(m_VAO, attributeLocation);
		glVertexArrayAttribFormat(
			m_VAO,
			attributeLocation,
			element.GetComponentCount(),
			VertexBufferLayout::ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized,
			element.Offset
		);

		glVertexArrayAttribBinding(m_VAO, attributeLocation, m_VertexBindingIndex);
	}

	m_VertexBuffers.emplace_back(vertexBuffer);
	m_VertexBindingIndex++;
}


void VertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	glVertexArrayElementBuffer(m_VAO, indexBuffer->GetID());
	m_IndexBuffer = indexBuffer;
}
