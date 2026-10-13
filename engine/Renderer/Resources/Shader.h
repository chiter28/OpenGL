#pragma once

#include <unordered_map>
#include <array>
#include <string>
#include <utility>
#include <cstdint>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:

	explicit Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();



	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;



	void Bind() const;
	void Unbind() const;



	void Release();



	void SetUniformBlockBinding(const std::string& blockName, uint32_t bindingPoint) const;



	void SetFloat(const std::string& name, float fval);
	void SetInt(const std::string& name, int ival);
	void SetVec3(const std::string& name, glm::vec3 vec3);
	void SetVec4(const std::string& name, glm::vec4 vec4);
	void SetMat4(const std::string& name, const glm::mat4& mat4);
	void SetMat3(const std::string& name, const glm::mat3& mat3);

private:
	using StageSource = std::pair<GLenum, std::string>;

	std::string ReadFile(const std::string& filePath);
	
	void Compile(const std::array<StageSource, 2>& shaderSources);

	int GetUniformLocation(const std::string& name) const;

private:
	GLuint m_Program = 0;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;

};
