#include "Texture.h"

#include <iostream>

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path) 
	: m_Buffer(nullptr), m_ID(0), m_Width(0), m_Height(0), m_ColorDepth(0)
{
	// flips texture upside down as opengl expects first pixel to be bottom left as apposed to top left
	stbi_set_flip_vertically_on_load(1);
	m_Buffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_ColorDepth, 4);
	if (!m_Buffer) std::cout << "Image failed to load." << std::endl;

	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // horizontal wrap
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // vertical wrap

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer);

	unbind();

	if (m_Buffer) stbi_image_free(m_Buffer);
}

Texture::Texture() :
	m_ID(0), m_Buffer(nullptr), m_Width(0), m_Height(0), m_ColorDepth(0)
{
}

Texture::~Texture() {
	glDeleteTextures(1, &m_ID);
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::bindToSlot(unsigned int slot) const {
	glBindTextureUnit(slot, m_ID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool operator==(const Texture& tex1, const Texture& tex2) {
	return tex1.m_ID == tex2.m_ID;
}