#pragma once

#include <glm/glm.hpp>

class BaseLight
{
	BaseLight() = default;


private:
	glm::vec3 m_LightColor = { 1.0f, 1.0f, 1.0f };
	float m_AmbientIntensity = 1.0f;
};