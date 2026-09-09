#pragma once

#include <string>
#include <vector>
#include <span>
#include <memory>

#include <glm/glm.hpp>
#include "Layout.h"

enum class BufferType
{
	Position = 0,
	TexCoord,
	Count
};


class MultiVertexBuffer
{
public:

	MultiVertexBuffer();
	~MultiVertexBuffer();




	void SetData(BufferType type, const void* data, uint32_t size);
	void SetLayout(BufferType type, VertexAttribute vertexAttribute);

	uint32_t GetID(BufferType type) const { return m_Buffers[static_cast<size_t>(type)]; }
	const VertexBufferLayout& GetLayout(BufferType type) const { return m_Layouts[static_cast<size_t>(type)]; }

private:
	uint32_t m_Buffers[static_cast<size_t>(BufferType::Count)];
	VertexBufferLayout m_Layouts[static_cast<size_t>(BufferType::Count)];
};




class VertexBuffer
{
public:

	template<typename T>
	static std::shared_ptr<VertexBuffer> Create(const std::vector<T>& data)
	{
		return std::make_shared<VertexBuffer>(data);
	}

	template<typename T>
	VertexBuffer(const std::vector<T>& data)
		: VertexBuffer(data.data(), static_cast<uint32_t>(data.size() * sizeof(T)), BufferLayoutTraits<T>::Get()) {}

	VertexBuffer(const void* data, uint32_t size, const VertexBufferLayout& layout = {});

	~VertexBuffer();

	void SetLayout(const std::initializer_list<VertexAttribute>& vertexAttributes);
	void SetLayout(const VertexBufferLayout& layout) { m_Layout = layout; }


	uint32_t GetID() const { return m_VBO; }
	const VertexBufferLayout& GetLayout() const { return m_Layout; }

private:
	uint32_t m_VBO;
	VertexBufferLayout m_Layout;
};





class IndexBuffer
{
public:

	IndexBuffer(std::span<uint32_t> indexBuffer);
	~IndexBuffer();

	uint32_t GetID() const { return m_IBO; }

private:
	uint32_t m_IBO;
};
