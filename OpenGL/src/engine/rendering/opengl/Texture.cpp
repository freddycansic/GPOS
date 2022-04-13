#include "Texture.h"

#include <iostream>
#include <stdexcept>

#include "stb_image/stb_image.h"
#include "engine/Debug.h"

Texture::Texture(const std::string& path)
{
	// flips texture upside down as opengl expects first pixel to be bottom left as apposed to top left
	stbi_set_flip_vertically_on_load(1);
	m_Buffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_ColorDepth, 4);
	if (!m_Buffer) throw std::runtime_error("Image failed to load!");

	GLAPI(glGenTextures(1, &m_ID));
	GLAPI(glBindTexture(GL_TEXTURE_2D, m_ID));

	GLAPI(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLAPI(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLAPI(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); // horizontal wrap
	GLAPI(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); // vertical wrap

	GLAPI(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Buffer));

	GLAPI(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_Buffer) stbi_image_free(m_Buffer);
}

Texture::~Texture() {
	GLAPI(glDeleteTextures(1, &m_ID));
}

void Texture::bind() const {
	GLAPI(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void Texture::bindToSlot(unsigned int slot) const {
	//GLAPI(glBindTextureUnit(slot, m_ID));
	GLAPI(glActiveTexture(GL_TEXTURE0 + slot));
	GLAPI(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void Texture::unbind() const
{
	GLAPI(glBindTexture(GL_TEXTURE_2D, 0));
}

bool operator==(const Texture& tex1, const Texture& tex2) {
	return tex1.m_ID == tex2.m_ID;
}