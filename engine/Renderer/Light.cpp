#include "Light.h"

#include "Renderer/Shader.h"


void BaseLight::Bind(Shader& shader) const
{
	shader.SetVec3("u_Light.Color", m_Color);
	shader.SetFloat("u_Light.AmbientIntensity", m_AmbientIntensity);
}


void DirectionalLight::Bind(Shader& shader) const
{
	BaseLight::Bind(shader);
	shader.SetVec3("u_Light.Direction", -m_ViewDirection);
	shader.SetFloat("u_Light.DiffuseIntensity", m_DiffuseIntensity);
	shader.SetFloat("u_Light.SpecularIntensity", m_SpecularIntensity);
}

void DirectionalLight::CalculateViewDir(const glm::mat4& view)
{
	m_ViewDirection = glm::normalize(glm::mat3(view) * m_WorldDirection);
}
