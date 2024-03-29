#include "Texture.h"

#include <iostream>

#include "GL/glew.h"
#include "stb/stb_image.h"

#include "engine/Debug.h"

Texture::Texture(const std::string& path)
{
	stbi_set_flip_vertically_on_load(0);
	m_Buffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_ColorDepth, 4);

	if (!m_Buffer)
	{
		std::cout << "Image from path " + (path.empty() ? "NULL" : path) + " failed to load!" << std::endl;
		m_Buffer = stbi_load("res/textures/texture_not_found.png", &m_Width, &m_Height, &m_ColorDepth, 4);

		ASSERT_WITH_MSG(m_Buffer, "Default texture misplaced from res/textures/texture_not_found.png!")
	}

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
	this->m_Handle = tex.m_Handle;

	tex.m_Handle = 0;
	tex.m_ID = 0;

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