#pragma once

#include <glm/glm.hpp>

class Shader;

class BaseLight
{
public:
	BaseLight() = default;

	glm::vec3 m_Color = { 1.0f, 1.0f, 1.0f };
	float m_AmbientIntensity = 0.1f;

	virtual void Bind(Shader& shader) const;
};


class DirectionalLight : public BaseLight
{
public:
	glm::vec3 m_WorldDirection = { -1.0f, 0.0f, 0.0f };
	float m_DiffuseIntensity = 0.9f;
	float m_SpecularIntensity = 1.0f;

	virtual void Bind(Shader& shader) const override;

	void CalculateViewDir(const glm::mat4& view);

	const glm::vec3& GetViewDir() { return m_ViewDirection; }

private:
	glm::vec3 m_ViewDirection;
};