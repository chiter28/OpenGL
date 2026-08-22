#pragma once

#include <string>
#include <glad/glad.h>
#include "VertexArray.h"
#include "../Utils.h"
#include <vector>
#include <random>
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
	friend class VertexBuffer;

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
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type);
	
private:
	struct BufferElement
	{
		VertexElement Attribute;
		ShaderDataType Type = ShaderDataType::None;
		GLuint Size = 0;
		GLuint Offset = 0;
		bool Normalized = false;

		uint32_t GetComponentCount() const;
	};

	std::vector<BufferElement> m_BufferElements;
	uint32_t m_Stride = 0;
};




class VertexBuffer
{
public:
	friend class VertexArray;

	VertexBuffer(const std::vector<Vertex>& vertices);
	~VertexBuffer();
	void SetLayout(const std::initializer_list<VertexElement>& bufferElements);

	void Bind() const;
	void Unbind() const;

private:
	uint32_t m_VBO;
	VertexBufferLayout m_Layout;
};





class IndexBuffer
{
public:
	friend class VertexArray;

	IndexBuffer(std::span<uint32_t> indexBuffer);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
private:
	uint32_t m_IBO;
};
