#pragma once
#include <string>

#include "stb_image.h"


class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(uint32_t slot) const;

private:
	uint32_t m_ID;
};