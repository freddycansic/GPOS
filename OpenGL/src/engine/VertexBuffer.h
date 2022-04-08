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
	void setSubData(unsigned int offset, size_t dataSize, const void* data) const;

private:
	unsigned int m_ID;
};

