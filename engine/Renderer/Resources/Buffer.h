#pragma once

#include <string>
#include <vector>
#include <span>
#include <memory>
#include <cstddef>

#include <glm/glm.hpp>

#include "Layout.h"



class VertexBuffer
{
public:

	VertexBuffer(const void* data, uint32_t size);

	template<typename T>
	static std::shared_ptr<VertexBuffer> Create(const std::vector<T>& data)
	{
		return std::make_shared<VertexBuffer>(data);
	}

	template<typename T>
	VertexBuffer(const std::vector<T>& data)
		: VertexBuffer(data.data(), static_cast<uint32_t>(data.size() * sizeof(T)), BufferLayoutTraits<T>::Get())
	{}

	VertexBuffer(const void* data, uint32_t size, VertexBufferLayout layout);

	~VertexBuffer();



	// Disable copying 
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	// Allowing movement
	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;


	void Release();
	void SetLayout(const std::initializer_list<VertexAttribute>& vertexAttributes);
	void SetLayout(const VertexBufferLayout& layout) { m_Layout = layout; }

	uint32_t GetID() const { return m_ID; }
	const VertexBufferLayout& GetLayout() const { return m_Layout; }

private:
	uint32_t m_ID = 0;
	VertexBufferLayout m_Layout;
};





class IndexBuffer
{
public:

	// Disable copying 
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	// Allowing movement
	IndexBuffer(IndexBuffer&& other) noexcept;
	IndexBuffer& operator=(IndexBuffer&& other) noexcept;

	IndexBuffer(std::span<uint32_t> indexBuffer);
	~IndexBuffer();

	void Release();
	uint32_t GetID() const { return m_ID; }

private:
	uint32_t m_ID = 0;
};





class UniformBuffer
{
public:
	explicit UniformBuffer(uint32_t size);
	~UniformBuffer();

	UniformBuffer(const UniformBuffer&) = delete;
	UniformBuffer& operator=(const UniformBuffer&) = delete;

	UniformBuffer(UniformBuffer&&) = delete;
	UniformBuffer& operator=(UniformBuffer&&) = delete;

	void SetData(std::span<const std::byte> data) const;
	void Bind(uint32_t bindingPoint) const;

private:
	uint32_t m_ID = 0;
	uint32_t m_Size = 0;
};


