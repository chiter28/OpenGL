#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>


class Shader;
class Texture;


class Material
{
public:
	Material(std::shared_ptr<Shader> shader = nullptr);
	~Material() = default;

	void Bind();

	void SetFloat(const std::string& name, float value);
	void SetInt(const std::string& name, int value);
	void SetVec3(const std::string& name, glm::vec3 value);
	void SetVec4(const std::string& name, glm::vec4 value);
	void SetTexture(const std::string& name, std::shared_ptr<Texture> texture, uint32_t slot = 0);
	void SetTransparent(bool transparent) { m_IsTransparent = transparent; }

	bool IsTransparent() const { return m_IsTransparent; }

private:
	struct TextureBinding
	{
		std::shared_ptr<Texture> texture;
		uint32_t slot;
	};

private:
	std::shared_ptr<Shader> m_Shader;
	// Shader Uniforms
	std::unordered_map<std::string, float> m_Floats;
	std::unordered_map<std::string, int> m_Ints;
	std::unordered_map<std::string, glm::vec3> m_Vec3s;
	std::unordered_map<std::string, glm::vec4> m_Vec4s;
	std::unordered_map<std::string, TextureBinding> m_Textures;
	bool m_IsTransparent = false;
};