#pragma once

#include <GL/glew.h>
#include <string>

class Texture
{
private:
	unsigned int m_ID;
	unsigned char* m_Buffer;
	int m_Width, m_Height, m_ColorDepth;

public:
	Texture(const std::string& path);
	~Texture();

	Texture(const Texture& tex) = delete;
	Texture& operator=(const Texture& tex) = delete;

	void bind() const;
	void bindToSlot(unsigned int slot) const;
	void unbind() const;

	unsigned int getID() const { return m_ID; }
};