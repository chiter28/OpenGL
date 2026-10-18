#include "Shader.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>


namespace
{
	// Shader resources deleter
	struct ShaderCompilationResources
	{
		GLuint Program = 0;
		std::array<GLuint, 2> ShaderIDs{ 0 };
		size_t ShaderCount = 0;

		ShaderCompilationResources() = default;
		ShaderCompilationResources(const ShaderCompilationResources&) = delete;
		ShaderCompilationResources& operator=(const ShaderCompilationResources&) = delete;

		~ShaderCompilationResources()
		{
			for (size_t i = 0; i < ShaderCount; ++i)
			{
				glDeleteShader(ShaderIDs[i]);
			}

			if (Program != 0)
			{
				glDeleteProgram(Program);
			}
		}
	};
}








Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	const std::array<StageSource, 2> sources = {
		StageSource{ GL_VERTEX_SHADER, ReadFile(vertexPath) },
		StageSource{ GL_FRAGMENT_SHADER, ReadFile(fragmentPath) }
    };
	
	Compile(sources);
}

Shader::~Shader()
{
	Release();
}





Shader::Shader(Shader&& other) noexcept
	: m_Program(std::exchange(other.m_Program, 0)),
	  m_UniformLocationCache(std::move(other.m_UniformLocationCache))
{}

Shader& Shader::operator=(Shader&& other) noexcept
{
	if (this == &other)
		return *this;

	Release();

	m_Program = std::exchange(other.m_Program, 0);
	m_UniformLocationCache = std::move(other.m_UniformLocationCache);
	return *this;
}






void Shader::Bind() const
{
	if (m_Program != 0)
		glUseProgram(m_Program);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}





void Shader::Release()
{
	if (m_Program != 0)
	{
		glDeleteProgram(m_Program);
		m_Program = 0;
	}
	m_UniformLocationCache.clear();
}





void Shader::SetUniformBlockBinding(const std::string& blockName, uint32_t bindingPoint) const
{
	if (m_Program == 0)
	{
		throw std::runtime_error("Shader program is not initialized");
	}

	const GLuint blockIndex = glGetUniformBlockIndex(m_Program, blockName.c_str());

	if (blockIndex == GL_INVALID_INDEX)
	{
		throw std::runtime_error("Uniform block not found " + blockName);
	}

	glUniformBlockBinding(m_Program, blockIndex, bindingPoint);
}











std::string Shader::ReadFile(const std::string& filePath)
{
	std::ifstream in(filePath, std::ios::in | std::ios::binary);
	if (!in)
	{
		throw std::runtime_error("Cannot open shader file: " + filePath);
	}

	in.seekg(0, std::ios::end); // translate pointer to the end of file
	const std::streampos endPosition = in.tellg(); // get position of pointer in bytes (file size in bytes)
	
	const std::streamoff size = static_cast<std::streamoff>(endPosition);
	if (size < 0)
	{
		throw std::runtime_error("Cannot determine shader file size: " + filePath);
	}
	if (size == 0)
	{
		throw std::runtime_error("Shader file is empty: " + filePath);
	}
	if (!std::in_range<size_t>(size) || !std::in_range<std::streamsize>(size))
	{
		throw std::length_error("Shader is too large: " + filePath);
	}

	std::string result(static_cast<size_t>(size), '\0');

	in.seekg(0, std::ios::beg); // return pointer to begin of file
	in.read(result.data(), static_cast<std::streamsize>(size));

	if (!in)
	{
		throw std::runtime_error("Cannot read shader file: " + filePath);
	}

	return result;
}









void Shader::Compile(const std::array<StageSource, 2>& shaderSources)
{
	ShaderCompilationResources resources{};

	resources.Program = glCreateProgram();
	if (resources.Program == 0)
	{
		throw std::runtime_error("Failed to create shader Program");
	}

	for (const auto& [type, source] : shaderSources)
	{
		const char* stageName = type == GL_VERTEX_SHADER ? "Vertex" : "Fragment";

		GLuint shader = glCreateShader(type);
		if (shader == 0)
		{
			throw std::runtime_error(std::string("Failed to create shader: ") + stageName);
		}

		resources.ShaderIDs[resources.ShaderCount++] = shader;

		const GLchar* srcCstr = source.c_str();
		glShaderSource(shader, 1, &srcCstr, nullptr);
		glCompileShader(shader);
	
		GLint isSuccessful = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isSuccessful);
		if (!isSuccessful)
		{
			int maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength > 0 ? maxLength : 1, '\0');
			glGetShaderInfoLog(shader, maxLength, nullptr, infoLog.data());

			throw std::runtime_error(stageName + std::string("shader compilation failed: ") + infoLog.data());
		}

		glAttachShader(resources.Program, shader);
	}

	glLinkProgram(resources.Program);

	GLint isLinked = 0;
	glGetProgramiv(resources.Program, GL_LINK_STATUS, &isLinked);
	if (!isLinked) {
		int maxLength = 0;
		glGetProgramiv(resources.Program, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> infoLog(maxLength > 0 ? maxLength : 1, '\0');
		glGetProgramInfoLog(resources.Program, maxLength, nullptr, infoLog.data());

		throw std::runtime_error(std::string("Shader linking failed: ") + infoLog.data());
	}

	for (size_t i = 0; i < resources.ShaderCount; ++i)
	{
		glDetachShader(resources.Program, resources.ShaderIDs[i]);
	}

	Release();

	m_Program = std::exchange(resources.Program, 0);
}









///////////////////////
// Setting Uniforms //
/////////////////////
int Shader::GetUniformLocation(const std::string& name) const
{
	if (m_Program == 0)
		return -1;
	
	auto it = m_UniformLocationCache.find(name);
 
	if (it != m_UniformLocationCache.end()) {
		return it->second;
	} 
	
	GLint location = glGetUniformLocation(m_Program, name.c_str());
	m_UniformLocationCache.emplace(name, location);
	if (location == -1) {
		std::cerr << "Uniform unavailable: " << name << '\n';
		return -1;
	}
	return location;
}


void Shader::SetFloat(const std::string& name, float fval)
{
	int location = GetUniformLocation(name);
	if (location == -1)
	{
		return;
	}
	glProgramUniform1f(m_Program, location, fval);
}

void Shader::SetInt(const std::string& name, int ival)
{
	int location = GetUniformLocation(name);
	if (location == -1)
	{
		return;
	}
	glProgramUniform1i(m_Program, location, ival);
}

void Shader::SetVec3(const std::string& name, glm::vec3 vec3)
{
	int location = GetUniformLocation(name);
	if (location == -1)
	{
		return;
	}
	glProgramUniform3f(m_Program, location, vec3.x, vec3.y, vec3.z);
}

void Shader::SetVec4(const std::string& name, glm::vec4 vec4)
{
	int location = GetUniformLocation(name);
	if (location == -1)
	{
		return;
	}
	glProgramUniform4f(m_Program, location, vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat4)
{
	int location = GetUniformLocation(name);
	if (location == -1)
	{
		return;
	}
	glProgramUniformMatrix4fv(m_Program, location, 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat3)
{
	int location = GetUniformLocation(name);
	if (location == -1)
	{
		return;
	}
	glProgramUniformMatrix3fv(m_Program, location, 1, GL_FALSE, glm::value_ptr(mat3));
}





