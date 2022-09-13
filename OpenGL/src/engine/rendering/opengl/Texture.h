#pragma once

#include <string>

class Texture
{
public:
	explicit Texture(const std::string& path);
	Texture() = default;
	~Texture();

	Texture(const Texture& tex) = delete;
	Texture& operator=(const Texture& tex) = delete;

	Texture(Texture&& tex) noexcept;
	Texture& operator=(Texture&& tex) noexcept;

	void bind() const;
	void bindToSlot(unsigned int slot) const;
	void unbind() const;

	[[nodiscard]] unsigned int getID() const { return m_ID; }
	[[nodiscard]] uint64_t getHandle() const { return m_Handle; }

private:
	unsigned int m_ID;
	unsigned char* m_Buffer;
	int m_Width, m_Height, m_ColorDepth;
	uint64_t m_Handle;
};