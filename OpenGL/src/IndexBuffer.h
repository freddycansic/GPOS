#pragma once

#include <GL/glew.h>

class IndexBuffer
{
public:
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();
	void bind() const;
	void unbind() const;

	unsigned int getCount() const { return m_Count; }

private:
	unsigned int m_ID;
	unsigned int m_Count;
};

