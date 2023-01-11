#pragma once

#include <GL/glew.h>

class IndexBuffer
{
public:

	IndexBuffer(const void* data, GLenum type, size_t count);
	~IndexBuffer();

	IndexBuffer(const IndexBuffer& tex) = delete;
	IndexBuffer& operator=(const IndexBuffer& tex) = delete;

	void setSubData(unsigned int offset, size_t count, const void* data) const;
	void setData(size_t dataSizeBytes, const void* data) const;
	void bind() const;
	void unbind() const;

	[[nodiscard]] size_t getCount() const { return m_Count; }
	[[nodiscard]] GLenum getType() const { return m_Type; }

private:
	unsigned int m_ID = 0;
	size_t m_Count = 0;
	GLenum m_Type = 0;
};

