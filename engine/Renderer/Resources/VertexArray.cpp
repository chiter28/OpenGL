#include "VertexArray.h"

#include <utility>
#include <limits>


#include <glad/glad.h>

static uint32_t GetAttributeLocation(VertexAttribute attrib)
{
	static constexpr uint32_t InvalidAttributeLocation = std::numeric_limits<uint32_t>::max();

	switch (attrib)
	{
		case VertexAttribute::Position:	  return 0;
		case VertexAttribute::Color:	  return 1;
		case VertexAttribute::TexCoord:	  return 2;
		case VertexAttribute::Instance:	  return 3;
		case VertexAttribute::Normal:	  return 4;
		case VertexAttribute::Tangent:	  return 5;
		default: return InvalidAttributeLocation;
	}
}


VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_ID);
}

VertexArray::VertexArray(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	glCreateVertexArrays(1, &m_ID);
	AddVertexBuffer(vertexBuffer);
	AddIndexBuffer(indexBuffer);
}

VertexArray::VertexArray(VertexArray&& other) noexcept
	: m_ID(std::exchange(other.m_ID, 0)),
	  m_VertexBuffers(std::move(other.m_VertexBuffers)),
	  m_MultiVertexBuffer(std::move(other.m_MultiVertexBuffer)),
	  m_IndexBuffer(std::move(other.m_IndexBuffer)),
	  m_VertexBindingIndex(std::exchange(other.m_VertexBindingIndex, 0))
{}

VertexArray& VertexArray::operator=(VertexArray && other) noexcept
{
	if (this == &other)
		return *this;

	Release();

	m_ID = std::exchange(other.m_ID, 0);
	m_VertexBuffers = std::move(other.m_VertexBuffers);
	m_MultiVertexBuffer = std::move(other.m_MultiVertexBuffer);
	m_IndexBuffer = std::move(other.m_IndexBuffer);
	m_VertexBindingIndex = std::exchange(other.m_VertexBindingIndex, 0);

	return *this;
}

VertexArray::~VertexArray()
{
	Release();
}

void VertexArray::Release()
{
	if (m_ID != 0)
	{
		glDeleteVertexArrays(1, &m_ID);
		m_ID = 0;
	}
	m_VertexBuffers.clear();
	m_MultiVertexBuffer = nullptr;
	m_IndexBuffer = nullptr;
	m_VertexBindingIndex = 0;
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArray::AddMultiVertexBuffer(BufferType type, const std::shared_ptr<MultiVertexBuffer>& multiVertexBuffer)
{
	glVertexArrayVertexBuffer(m_ID, m_VertexBindingIndex, multiVertexBuffer->GetID(type), 0, multiVertexBuffer->GetLayout(type).GetStride());

	for (const VertexBufferLayout::BufferElement& element : multiVertexBuffer->GetLayout(type).GetElements())
	{
		uint32_t attributeLocation = GetAttributeLocation(element.Attribute);

		glEnableVertexArrayAttrib(m_ID, attributeLocation);
		glVertexArrayAttribFormat(
			m_ID,
			attributeLocation,
			element.GetComponentCount(),
			VertexBufferLayout::ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized,
			element.Offset
		);

		glVertexArrayAttribBinding(m_ID, attributeLocation, m_VertexBindingIndex);
	}

	if (m_MultiVertexBuffer != multiVertexBuffer)
	{
		m_MultiVertexBuffer = multiVertexBuffer;
	}

	m_VertexBindingIndex++;
}


void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, uint32_t divisor)
{
	glVertexArrayVertexBuffer(m_ID, m_VertexBindingIndex, vertexBuffer->GetID(), 0, vertexBuffer->GetLayout().GetStride());

	if (divisor > 0)
		glVertexArrayBindingDivisor(m_ID, m_VertexBindingIndex, divisor);

	for (const VertexBufferLayout::BufferElement& element : vertexBuffer->GetLayout().GetElements())
	{
		uint32_t attributeLocation = GetAttributeLocation(element.Attribute);

		glEnableVertexArrayAttrib(m_ID, attributeLocation);
		glVertexArrayAttribFormat(
			m_ID,
			attributeLocation,
			element.GetComponentCount(),
			VertexBufferLayout::ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized,
			element.Offset
		);

		glVertexArrayAttribBinding(m_ID, attributeLocation, m_VertexBindingIndex);
	}

	m_VertexBuffers.emplace_back(vertexBuffer);
	++m_VertexBindingIndex;
}


void VertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	glVertexArrayElementBuffer(m_ID, indexBuffer->GetID());
	m_IndexBuffer = indexBuffer;
}
