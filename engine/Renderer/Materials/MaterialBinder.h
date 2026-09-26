#pragma once

#include "Renderer/Resources/Texture.h"
#include "Renderer/Resources/Buffer.h"

class Shader;
class Material;

class MaterialBinder
{
public:
	MaterialBinder();

	void ConfigureShader(Shader& shader) const;
	void Apply(const Material& material) const;

private:
	Texture m_WhiteTexture;
	UniformBuffer m_MaterialBuffer;
};