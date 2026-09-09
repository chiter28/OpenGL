#include "Texture.h"
#include <iostream>
#include <glad/glad.h>


Texture::Texture(const uint8_t* buffer, uint32_t length)
{
	int32_t width, height, channels;
	stbi_set_flip_vertically_on_load(true);

	stbi_uc* imageData = stbi_load_from_memory(buffer, length, &width, &height, &channels, 4);
	
	if (!imageData) {
		std::cerr << "Texture load (from buffer) failed : " << stbi_failure_reason() << std::endl;
		return;
	}

	GLenum internalFormat_GPU = GL_RGBA8;  // как хранить на GPU
	GLenum dataFormat_CPU = GL_RGBA;     // как лежат данные в CPU

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
	glTextureStorage2D(m_ID, 1, internalFormat_GPU, width, height);


	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


	glTextureSubImage2D(m_ID, 0, 0, 0, width, height, dataFormat_CPU, GL_UNSIGNED_BYTE, imageData);
	stbi_image_free(imageData);


}

Texture::Texture(const uint8_t* data, uint32_t width, uint32_t height)
{
	
	GLenum internalFormat_GPU = GL_RGBA8;  // как хранить на GPU
	GLenum dataFormat_CPU = GL_RGBA;     // как лежат данные в CPU

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
	glTextureStorage2D(m_ID, 1, internalFormat_GPU, width, height);


	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


	glTextureSubImage2D(m_ID, 0, 0, 0, width, height, dataFormat_CPU, GL_UNSIGNED_BYTE, data);
}




Texture::Texture(const std::string& path)
{
	stbi_set_flip_vertically_on_load(true);

	int32_t width, height;
	int32_t beatsPerPixel;
	stbi_uc* imageData = stbi_load(path.c_str(), &width, &height, &beatsPerPixel, 4);

	if (!imageData) {
		std::cerr << "Texture load failed from " << path << " - " << stbi_failure_reason() << std::endl;
		return;
	}

	GLenum internalFormat_GPU = GL_RGBA8;  // как хранить на GPU
	GLenum dataFormat_CPU = GL_RGBA;     // как лежат данные в CPU

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
	glTextureStorage2D(m_ID, 1, internalFormat_GPU, width, height); // allocate memoty on GPU

	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER)
		;
	glTextureSubImage2D(m_ID, 0, 0, 0, width, height, dataFormat_CPU, GL_UNSIGNED_BYTE, imageData); // load image to that memory

	stbi_image_free(imageData);
}

Texture::~Texture()
{
	if (m_ID != 0)
		glDeleteTextures(1, &m_ID);
}

void Texture::Bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_ID);
}
