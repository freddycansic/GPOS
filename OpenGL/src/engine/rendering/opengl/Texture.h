#pragma once

#include <GL/glew.h>
#include <string>

class Texture
{
private:
	unsigned int m_ID = 0;
	unsigned char* m_Buffer = nullptr;
	int m_Width = 0, m_Height = 0, m_ColorDepth = 0;

public:
	Texture(const std::string& path);
	Texture() = default;
	~Texture();
	Texture(const Texture& tex) = delete;

	void bind() const;
	void bindToSlot(unsigned int slot) const;
	void unbind() const;

	friend bool operator==(const Texture& tex1, const Texture& tex2);

	unsigned int getID() const { return m_ID; }
};