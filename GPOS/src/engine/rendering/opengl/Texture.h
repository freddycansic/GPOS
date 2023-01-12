#pragma once

#include <string>
#include <unordered_map>

class Texture
{
public:
	explicit Texture(const char* path);
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
	unsigned int m_ID = 0;
	unsigned char* m_Buffer = nullptr;
	int m_Width = 0, m_Height = 0, m_ColorDepth = 0;
	uint64_t m_Handle = 0;
};