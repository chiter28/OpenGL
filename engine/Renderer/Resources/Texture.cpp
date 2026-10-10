#include "Texture.h"

#include <stb_image.h>
#include <glad/glad.h>

#include <iostream>
#include <utility>
#include <stdexcept>
#include <algorithm>




namespace
{
	GLint WrapToOpenGL(TextureWrap wrap)
	{
		switch (wrap)
		{
			case TextureWrap::Repeat:
				return GL_REPEAT;
			case TextureWrap::MirroredRepeat:
				return GL_MIRRORED_REPEAT;
			case TextureWrap::ClampToEdge:
				return GL_CLAMP_TO_EDGE;
		}

		throw std::invalid_argument("Invalid texture Wrap");
	}

	GLint MagFilterToOpenGL(TextureMagFilter filter)
	{
		switch (filter)
		{
			case TextureMagFilter::Linear:
				return GL_LINEAR;
			case TextureMagFilter::Nearest:
				return GL_NEAREST;
		}

		throw std::invalid_argument("Invalid Magnification filter");
	}

	GLint MinFilterToOpenGL(TextureMinFilter filter)
	{
		switch (filter)
		{
			case TextureMinFilter::Linear:
				return GL_LINEAR;
			case TextureMinFilter::Nearest:
				return GL_NEAREST;
			case TextureMinFilter::NearestMipMapNearest:
				return GL_NEAREST_MIPMAP_NEAREST;
			case TextureMinFilter::LinearMipMapNearest:
				return GL_LINEAR_MIPMAP_NEAREST;
			case TextureMinFilter::NearestMipMapLinear:
				return GL_NEAREST_MIPMAP_LINEAR;
			case TextureMinFilter::LinearMipMapLinear:
				return GL_LINEAR_MIPMAP_LINEAR;
		}

		throw std::invalid_argument("Invalid Minification filter");
	}



	GLsizei CalculateMipLevelCount(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
		{
			throw std::invalid_argument("Texture demensions must be positive");
		}

		uint32_t largestDimension = std::max(width, height);
		GLsizei levels = 1;

		while (largestDimension > 1)
		{
			largestDimension /= 2;
			++levels;
		}

		return levels;
	}
}


Texture::Texture(Texture&& other) noexcept
	: m_ID(std::exchange(other.m_ID, 0))
{}

Texture& Texture::operator=(Texture&& other) noexcept
{
	if (this == &other)
		return *this;
	Release();
	m_ID = std::exchange(other.m_ID, 0);
	return *this;
}

Texture::~Texture()
{
	Release();
}

void Texture::Release()
{
	if (m_ID != 0)
	{
		glDeleteTextures(1, &m_ID);
		m_ID = 0;
	}
}


void Texture::Bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_ID);
}

void Texture::Unbind(uint32_t slot)
{
	glBindTextureUnit(slot, 0);
}

// указатель на поток сжатых байт (например, файл PNG/JPG, загруженный в оперативную память)
Texture::Texture(const std::byte* buffer, uint32_t length, TextureColorSpace colorSpace, TextureSamplerDescription sampler)
{
	// sampler
	const GLint magFilter = MagFilterToOpenGL(sampler.MagFilter);
	const GLint minFilter = MinFilterToOpenGL(sampler.MinFilter);
	const GLint wrapU = WrapToOpenGL(sampler.WrapU);
	const GLint wrapV = WrapToOpenGL(sampler.WrapV);




	int32_t width, height, channels;
	stbi_set_flip_vertically_on_load(false);

	stbi_uc* imageData = stbi_load_from_memory((const stbi_uc*)(buffer), length, &width, &height, &channels, 4);
	
	if (!imageData) {
		std::cerr << "Texture load (from buffer) failed : " << stbi_failure_reason() << std::endl;
		return;
	}


	// MipMaps levels
	const bool usesMipmaps = minFilter != GL_LINEAR && minFilter != GL_NEAREST;
	const GLsizei levels = usesMipmaps ? CalculateMipLevelCount(width, height) : 1;



	// storage data format
	const GLenum internalFormat_GPU =
		(colorSpace == TextureColorSpace::sRGB) ? GL_SRGB8_ALPHA8 : GL_RGBA8;  // как хранить на GPU
	const GLenum dataFormat_CPU = GL_RGBA;     // как лежат данные в CPU

	


	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
	glTextureStorage2D(m_ID, levels, internalFormat_GPU, width, height);

	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, magFilter);
	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, minFilter);

	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, wrapU);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, wrapV);

	glTextureSubImage2D(m_ID, 0, 0, 0, width, height, dataFormat_CPU, GL_UNSIGNED_BYTE, imageData);

	if (usesMipmaps)
	{
		glGenerateTextureMipmap(m_ID);
	}


	stbi_image_free(imageData);
}


// копирует готовый массив пикселей в GPU
Texture::Texture(const uint8_t* data, uint32_t width, uint32_t height, TextureColorSpace colorSpace, TextureSamplerDescription sampler)
{
	// sampler
	const GLint magFilter = MagFilterToOpenGL(sampler.MagFilter);
	const GLint minFilter = MinFilterToOpenGL(sampler.MinFilter);
	const GLint wrapU = WrapToOpenGL(sampler.WrapU);
	const GLint wrapV = WrapToOpenGL(sampler.WrapV);


	// MipMaps levels
	const bool usesMipmaps = minFilter != GL_LINEAR && minFilter != GL_NEAREST;
	const GLsizei levels = usesMipmaps ? CalculateMipLevelCount(width, height) : 1;



	const GLenum internalFormat_GPU =
		(colorSpace == TextureColorSpace::sRGB) ? GL_SRGB8_ALPHA8 : GL_RGBA8;  // как хранить на GPU
	const GLenum dataFormat_CPU = GL_RGBA;     // как лежат данные в CPU

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
	glTextureStorage2D(m_ID, levels, internalFormat_GPU, width, height);


	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, magFilter);
	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, minFilter);

	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, wrapU);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, wrapV);



	glTextureSubImage2D(m_ID, 0, 0, 0, width, height, dataFormat_CPU, GL_UNSIGNED_BYTE, (const void*)data);

	if (usesMipmaps)
	{
		glGenerateTextureMipmap(m_ID);
	}

}




Texture::Texture(const std::string& path, TextureColorSpace colorSpace, TextureSamplerDescription sampler)
{
	// sampler
	const GLint magFilter = MagFilterToOpenGL(sampler.MagFilter);
	const GLint minFilter = MinFilterToOpenGL(sampler.MinFilter);
	const GLint wrapU = WrapToOpenGL(sampler.WrapU);
	const GLint wrapV = WrapToOpenGL(sampler.WrapV);



	stbi_set_flip_vertically_on_load(false);

	int32_t width, height;
	int32_t beatsPerPixel;
	stbi_uc* imageData = stbi_load(path.c_str(), &width, &height, &beatsPerPixel, 4);

	if (!imageData) {
		std::cerr << "Texture load failed from " << path << " - " << stbi_failure_reason() << std::endl;
		return;
	}



	// MipMaps levels
	const bool usesMipmaps = minFilter != GL_LINEAR && minFilter != GL_NEAREST;
	const GLsizei levels = usesMipmaps ? CalculateMipLevelCount(width, height) : 1;


	const GLenum internalFormat_GPU =
		(colorSpace == TextureColorSpace::sRGB) ? GL_SRGB8_ALPHA8 : GL_RGBA8;  // как хранить на GPU
	const GLenum dataFormat_CPU = GL_RGBA;     // как лежат данные в CPU 



	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
	glTextureStorage2D(m_ID, levels, internalFormat_GPU, width, height); // allocate memoty on GPU

	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, magFilter);
	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, minFilter);

	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, wrapU);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, wrapV);

	glTextureSubImage2D(m_ID, 0, 0, 0, width, height, dataFormat_CPU, GL_UNSIGNED_BYTE, imageData); // load image to that memory

	if (usesMipmaps)
	{
		glGenerateTextureMipmap(m_ID);
	}

	stbi_image_free(imageData);
}