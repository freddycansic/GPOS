#pragma once

#include <GL/glew.h>
#include <vector>

class VertexBuffer
{
public:
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer();
	void bind() const;
	void unbind() const;

private:
	unsigned int m_ID;
};

