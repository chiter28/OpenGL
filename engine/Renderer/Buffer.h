#pragma once

#include <string>
#include "../Utils.h"
#include <vector>
#include <span>

enum class VertexElement
{
	None = 0,
	Position,
	Color
};


struct alignas(4) Vertex
{
	vec3 position;
	vec3 color;

	Vertex() = default;

	Vertex(float xPos, float yPos, float zPos, float R, float G, float B)
		: position(xPos, yPos, zPos), color(R, G, B)
	{}
};



class VertexBufferLayout
{
public:
	friend class VertexArray;

	VertexBufferLayout() = default;

	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Mat3, Mat4,
		Bool
	};

	VertexBufferLayout(const std::initializer_list<VertexElement>& vertexElements);

private:
	static uint32_t GetShaderDataTypeSize(ShaderDataType type);
	static uint32_t ShaderDataTypeToOpenGLBaseType(ShaderDataType type);
	
private:
	struct BufferElement
	{
		VertexElement Attribute;
		ShaderDataType Type = ShaderDataType::None;
		uint32_t Size = 0;
		uint32_t Offset = 0;
		bool Normalized = false;

		uint32_t GetComponentCount() const;
	};

	std::vector<BufferElement> m_BufferElements;
	uint32_t m_Stride = 0;
};




class VertexBuffer
{
public:

	VertexBuffer(const std::vector<Vertex>& vertices);
	~VertexBuffer();
	void SetLayout(const std::initializer_list<VertexElement>& bufferElements);

	void Bind() const;
	void Unbind() const;

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

	void Bind() const;
	void Unbind() const;

	uint32_t GetID() const { return m_IBO; }

private:
	uint32_t m_IBO;
};
