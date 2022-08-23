#include "Texture.h"

#include <iostream>
#include <stdexcept>

#include "GL/glew.h"

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

	m_Handle = glGetTextureHandleARB(m_ID);
	GLAPI(glMakeTextureHandleResidentARB(m_Handle));

	GLAPI(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_Buffer) stbi_image_free(m_Buffer);
}

Texture::Texture(Texture&& tex) noexcept :
	m_ID(tex.m_ID), m_Buffer(tex.m_Buffer), m_Width(tex.m_Width), m_Height(tex.m_Height), m_ColorDepth(tex.m_ColorDepth), m_Handle(tex.m_Handle)
{
	tex.m_Handle = 0;
	tex.m_ID = 0;
}

Texture& Texture::operator=(Texture&& tex) noexcept
{
	this->m_ID = tex.m_ID;
	this->m_Buffer = tex.m_Buffer;
	this->m_Width = tex.m_Width;
	this->m_Height = tex.m_Height;
	this->m_ColorDepth = tex.m_ColorDepth;
	this->m_Handle = tex.m_Height;

	tex.m_ID = 0;
	tex.m_Handle = 0;

	return *this;
}

void Texture::bind() const {
	GLAPI(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void Texture::bindToSlot(unsigned int slot) const {
	GLAPI(glBindTextureUnit(slot, m_ID));
}

void Texture::unbind() const
{
	GLAPI(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture() {
	GLAPI(glDeleteTextures(1, &m_ID));
}