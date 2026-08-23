#include "Shader.h"
#include <fstream>
#include <iostream>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


Shader::Shader(const std::string& filePath)
{
	std::string source = ReadFile(filePath);
	auto shaders = PreProcess(source);
	Compile(shaders);
}

static GLenum ShaderTypeFromString(const std::string& type)
{
	if (type == "vertex") {
		return GL_VERTEX_SHADER;
	}
	if (type == "fragment" || type == "pixel") {
		return GL_FRAGMENT_SHADER;
	} else {
		std::cerr << "Unknown Shader type" << std::endl;
		return 0;
	}

}




std::string Shader::ReadFile(const std::string& filePath)
{
	std::string result;
	std::ifstream in(filePath, std::ios::in | std::ios::binary);
	if (in) {
		in.seekg(0, std::ios::end); // translate pointer to the end of file
		const auto size = in.tellg(); // get position of pointer in bytes
		if (size > 0) {
			result.resize(static_cast<size_t>(size));
		}
		in.seekg(0, std::ios::beg); // return pointer to begin of file

		in.read(result.data(), result.size());
		in.close();
	}

	return result;
}

std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
{
	std::unordered_map<GLenum, std::string> shaderSource;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);

	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos)
	{
		size_t eol = source.find_first_of("\r\n", pos);
		if (eol == std::string::npos) {
			std::cerr << "Syntax Error" << std::endl;
		}
		size_t begin = pos + typeTokenLength + 1;
		std::string type = source.substr(begin, eol - begin); // type - vertex or fragment
		if (!ShaderTypeFromString(type)) {
			std::cerr << "Invalid Shader type" << std::endl;
		}
		
		size_t nextLinePos = source.find_first_not_of("\r\n", eol);

		pos = source.find(typeToken, nextLinePos);

		shaderSource[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - nextLinePos);
	}

	return shaderSource;
}

void Shader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
{
	GLuint program = glCreateProgram();
	std::array<GLuint, 2> shaderIDs;
	int shaderIDsIndex = 0;
	for (auto& sh : shaderSources)
	{
		GLenum type = sh.first;
		const std::string& source = sh.second;

		GLuint shader = glCreateShader(type);

		const GLchar* srcCstr = source.c_str();
		glShaderSource(shader, 1, &srcCstr, nullptr);
		glCompileShader(shader);
	
		GLint isSuccessful = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isSuccessful);
		if (!isSuccessful)
		{
			int maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, nullptr, infoLog.data());

			glDeleteShader(shader);
			std::cerr << "Shader compilation Failure: " << infoLog.data() << std::endl;
			break;
		}

		glAttachShader(program, shader);
		shaderIDs[shaderIDsIndex++] = shader;
	}

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (!isLinked) {
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

		glDeleteProgram(program);
		for (auto id : shaderIDs)
		{
			glDeleteShader(id);
		}
		std::cerr << "Shader Program linking Failure: " << infoLog.data() << std::endl;
		return;
	}

	for (auto id : shaderIDs)
	{
		glDetachShader(program, id);
	}

	m_Program = program;
}

int Shader::GetUniformLocation(const std::string& name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	} 
	else {
		m_UniformLocationCache[name] = glGetUniformLocation(m_Program, name.c_str());
		return m_UniformLocationCache[name];
	}
}

void Shader::Bind() const
{
	glUseProgram(m_Program);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetFloat(const std::string& name, float value)
{
	int location = GetUniformLocation(name.c_str());
	if (location == -1) {
		std::cerr << "Error: uniform " << name << " not found" << std::endl;
		return;
	}
	glUniform1f(location, value);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& matrix)
{
	int location = GetUniformLocation(name.c_str());
	if (location == -1) {
		std::cerr << "Error: uniform " << name << " not found" << std::endl;
		return;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetInt(const std::string& name, int value)
{
	int location = GetUniformLocation(name.c_str());
	if (location == -1) {
		std::cerr << "Error: uniform " << name << " not found" << std::endl;
		return;
	}
	glUniform1i(location, value);
}





