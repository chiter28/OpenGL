#pragma once
#include <string>

#include "stb_image.h"


class Texture
{
public:
	Texture() = default;
	Texture(const uint8_t* buffer, uint32_t length);
	Texture(const uint8_t* data, uint32_t width, uint32_t height);
	explicit Texture(const std::string& path);
	
	~Texture();

	// Disable copying 
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	
	// Allowing movement
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;
	

	void Release();
	void Bind(uint32_t slot) const;


private:
	uint32_t m_ID = 0;
};