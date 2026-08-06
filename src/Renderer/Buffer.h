#pragma once

#include <string>
#include <glad/glad.h>


struct BufferLayout
{
	std::string Name;
	GLuint Type;
	GLuint Size;
	GLuint Offset;
	bool Normalized;
};




class VertexBuffer
{
public:

	VertexBuffer(uint32_t size, float* vertices);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

private:
	uint32_t m_VBO;
};
