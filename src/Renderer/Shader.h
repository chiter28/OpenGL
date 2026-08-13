#pragma once
#include <glad/glad.h>

#include <string>
#include <unordered_map>
#include "../Utils.h"


class Shader
{
public:
	Shader(const std::string& filePath);
	void Bind() const;
	void Unbind() const;

	void SetFloat(const std::string& name, float value);
	void SetMat4(const std::string& name, const mat4& matrix);

private:
	std::string ReadFile(const std::string& filePath);
	std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
	
	void Compile(std::unordered_map<GLenum, std::string> shaderSources);

	int GetUniformLocation(const std::string& name) const;

private:
	GLuint m_Program;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;

};
