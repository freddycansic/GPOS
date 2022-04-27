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
	void bind() const;
	void unbind() const;

	size_t getCount() const { return m_Count; }
	GLenum getType() const { return m_Type; }

private:
	unsigned int m_ID;
	size_t m_Count;
	GLenum m_Type;
};

