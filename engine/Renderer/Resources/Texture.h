#pragma once
#include <string>


#include "TextureTypes.h"

#include <cstddef>


class Texture
{
public:
	Texture() = default;
	Texture(const std::byte* buffer, size_t length, TextureColorSpace colorSpace = TextureColorSpace::Linear, TextureSamplerDescription sampler = {});
	Texture(const uint8_t* data, uint32_t width, uint32_t height, TextureColorSpace colorSpace= TextureColorSpace::Linear, TextureSamplerDescription sampler = {});
	explicit Texture(const std::string& path, TextureColorSpace colorSpace = TextureColorSpace::Linear, TextureSamplerDescription sampler = {});
	
	~Texture();

	// Disable copying 
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	
	// Allowing movement
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;
	

	void Release();
	void Bind(uint32_t slot) const;

	static void Unbind(uint32_t slot);

private:
	uint32_t m_ID = 0;
};