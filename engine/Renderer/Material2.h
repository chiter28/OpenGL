#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Texture.h"


class Material
{
public:
	glm::vec4 BaseColor{ 1.0f };

	float Metallic = 0.0f;
	float Roughness = 1.0f;


	
	glm::vec3 SpecularColor{ 1.0f };
	float Shininess = 400.0f;

	glm::vec3 Emissive{ 0.0f };

	std::shared_ptr<Texture> BaseColorTexture;
	std::shared_ptr<Texture> NormalTexture;
	std::shared_ptr<Texture> MetallicRoughnessTexture;
	std::shared_ptr<Texture> OcclusionTexture;
	std::shared_ptr<Texture> EmissiveTexture;

	bool Transparent = false;
};