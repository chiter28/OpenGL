#include "VertexArray.h"
#include "../Utils.h"


VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_VAO);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_VAO);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	glVertexArrayVertexBuffer(m_VAO, m_VertexBindingIndex, vertexBuffer->m_VBO, 0, sizeof(Vertex));

	for (const VertexBufferLayout::BufferElement& element : vertexBuffer->m_Layout.m_BufferElements)
	{

		glEnableVertexArrayAttrib(m_VAO, m_AttributeIndex);
		glVertexArrayAttribFormat(
			m_VAO,
			m_AttributeIndex,
			element.GetComponentCount(),
			vertexBuffer->m_Layout.ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized,
			element.Offset
		);

		glVertexArrayAttribBinding(m_VAO, m_AttributeIndex++, m_VertexBindingIndex);
	}

	m_VertexBuffers.emplace_back(vertexBuffer);
	m_VertexBindingIndex++;
}


void VertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	glVertexArrayElementBuffer(m_VAO, indexBuffer->m_IBO);
	m_IndexBuffer = indexBuffer;
}
