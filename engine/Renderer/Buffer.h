#pragma once

#include <string>
#include "../Utils.h"
#include <vector>
#include <span>

enum class VertexAttribute
{
	None = 0,
	Position,
	Color,
	TexCoord
};


struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 TexCoord;

	Vertex() = default;

	Vertex(float xPos, float yPos, float zPos,
		   float R, float G, float B,
		   float u, float v)
		: Position(xPos, yPos, zPos), Color(R, G, B), TexCoord(u, v)
	{}
};



class VertexBufferLayout
{
public:
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Mat3, Mat4,
		Bool
	};

	struct BufferElement
	{
		VertexAttribute Attribute;
		ShaderDataType Type = ShaderDataType::None;
		uint32_t Size = 0;
		uint32_t Offset = 0;
		bool Normalized = false;

		uint32_t GetComponentCount() const;
	};

	uint32_t GetStride() const { return m_Stride; }

public:
	VertexBufferLayout() = default;
	VertexBufferLayout(const std::initializer_list<VertexAttribute>& vertexElements);

	const std::vector<BufferElement>& GetElements() const { return m_BufferElements; }

	static uint32_t ShaderDataTypeToOpenGLBaseType(ShaderDataType type);
	static uint32_t GetShaderDataTypeSize(ShaderDataType type);

private:
	std::vector<BufferElement> m_BufferElements;
	uint32_t m_Stride = 0;
};




class VertexBuffer
{
public:

	VertexBuffer(const std::vector<Vertex>& vertices);
	~VertexBuffer();
	void SetLayout(const std::initializer_list<VertexAttribute>& bufferElements);

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
