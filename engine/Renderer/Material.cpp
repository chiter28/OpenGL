#include "Material.h"
#include "Shader.h";
#include "Texture.h";


Material::Material(std::shared_ptr<Shader> shader)
	: m_Shader(std::move(shader))
{}


void Material::Bind()
{
	if (!m_Shader)
		return;

	m_Shader->Bind();

	for (const auto& [name, value] : m_Floats) {
		m_Shader->SetFloat(name, value);
	}
	for (const auto& [name, value] : m_Ints) {
		m_Shader->SetInt(name, value);
	}
	for (const auto& [name, value] : m_Vec3s) {
		m_Shader->SetVec3(name, value);
	}
	for (const auto& [name, value] : m_Vec4s) {
		m_Shader->SetVec4(name, value);
	}

	for (const auto& [name, textureBinding] : m_Textures)
	{
		if (textureBinding.texture) {
			textureBinding.texture->Bind(textureBinding.slot);
			m_Shader->SetInt(name, textureBinding.slot);
		}
	}

}

void Material::SetFloat(const std::string& name, float value) {
	m_Floats[name] = value;
}

void Material::SetInt(const std::string& name, int value) {
	m_Ints[name] = value;
}

void Material::SetVec3(const std::string& name, glm::vec3 value) {
	m_Vec3s[name] = value;
}

void Material::SetVec4(const std::string& name, glm::vec4 value) {
	m_Vec4s[name] = value;
}

void Material::SetTexture(const std::string& name, std::shared_ptr<Texture> texture, uint32_t slot) {
	m_Textures[name] = TextureBinding{ texture, slot };
}
