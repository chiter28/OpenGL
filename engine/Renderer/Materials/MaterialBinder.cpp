#include "MaterialBinder.h"

#include "Renderer/Resources/Shader.h"
#include "Material.h"


#include <cstdint>
#include <cstddef>
#include <type_traits>


namespace
{
	constexpr uint32_t BaseColorUnit = 0;
	constexpr uint32_t MetallicRoughnessUnit = 1;

	constexpr uint32_t MaterialBindingPoint = 0;

	constexpr uint8_t WhitePixel[] = { 255, 255, 255, 255 };
	

	// Упаковка для GPU
	struct alignas(16) MaterialUniformData
	{
		float BaseColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float Metallic = 0.0f;
		float Roughness = 1.0f;
		float _padding[2] = { 0 };
	};

	static_assert(sizeof(float) == 4);
	static_assert(std::is_standard_layout_v<MaterialUniformData>);
	static_assert(std::is_trivially_copyable_v<MaterialUniformData>);
	static_assert(offsetof(MaterialUniformData, BaseColor) == 0);
	static_assert(offsetof(MaterialUniformData, Metallic) == 16);
	static_assert(offsetof(MaterialUniformData, Roughness) == 20);
	static_assert(sizeof(MaterialUniformData) == 32);
}




MaterialBinder::MaterialBinder()
	: m_WhiteTexture(WhitePixel, 1, 1, TextureColorSpace::Linear),
	  m_MaterialBuffer(sizeof(MaterialUniformData))
{
	
}


void MaterialBinder::ConfigureShader(Shader& shader) const
{
	shader.SetInt("u_AlbedoMap", static_cast<int>(BaseColorUnit));
	shader.SetInt("u_MetallicRoughnessMap", static_cast<int>(MetallicRoughnessUnit));
	shader.SetUniformBlockBinding("MaterialBlock", MaterialBindingPoint);
}


void MaterialBinder::Apply(const Material& material) const
{
	MaterialUniformData materialData
	{
		.BaseColor =
		{
			material.BaseColor.r,
			material.BaseColor.g,
			material.BaseColor.b,
			material.BaseColor.a,
		},
		.Metallic = material.Metallic,
		.Roughness = material.Roughness
	};
	m_MaterialBuffer.SetData(std::as_bytes(std::span{ &materialData, 1 }));
	m_MaterialBuffer.Bind(MaterialBindingPoint);


	const Texture& baseColorTexture = material.BaseColorTexture ? *material.BaseColorTexture : m_WhiteTexture;
	const Texture& metallicRoughnessTexture = material.MetallicRoughnessTexture ? *material.MetallicRoughnessTexture : m_WhiteTexture;
	
	baseColorTexture.Bind(BaseColorUnit);
	metallicRoughnessTexture.Bind(MetallicRoughnessUnit);
}
