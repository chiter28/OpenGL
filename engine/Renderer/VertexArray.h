	#pragma once

#include <vector>
#include <memory>
#include "Buffer.h"


class VertexArray
{
public:
	VertexArray();
	VertexArray(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer);

	// Disable copying 
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	// Allowing movement
	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	~VertexArray();

	void Release();

	void Bind() const;



	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, uint32_t divisor = 0);
	void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	void AddMultiVertexBuffer(BufferType type, const std::shared_ptr<MultiVertexBuffer>& multiVertexBuffer);

private:
	uint32_t m_ID = 0;
	std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
	std::shared_ptr<MultiVertexBuffer> m_MultiVertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
	uint32_t m_VertexBindingIndex = 0;
};
