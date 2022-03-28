#pragma once

#include <GL/glew.h>

class IndexBuffer
{
public:
	IndexBuffer(const void* data, GLenum type, size_t count);
	~IndexBuffer();
	void bind() const;
	void unbind() const;

	size_t getCount() const { return m_Count; }
	GLenum getType() const { return m_Type; }

private:
	unsigned int m_ID;
	size_t m_Count;
	GLenum m_Type;
};

