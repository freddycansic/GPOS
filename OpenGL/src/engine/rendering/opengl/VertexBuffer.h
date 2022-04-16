#pragma once

#include <GL/glew.h>
#include <vector>

class VertexBuffer
{
private:
	unsigned int m_ID;

public:
	VertexBuffer(const void* data, size_t size);
	VertexBuffer();
	~VertexBuffer();

	VertexBuffer(const VertexBuffer& tex) = delete;
	VertexBuffer& operator=(const VertexBuffer& tex) = delete;

	void bind() const;
	void unbind() const;
	void setSubData(unsigned int offset, size_t dataSize, const void* data) const;
};

