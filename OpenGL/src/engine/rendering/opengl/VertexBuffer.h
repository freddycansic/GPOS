#pragma once

#include <GL/glew.h>
#include <vector>

class VertexBuffer
{
private:
	unsigned int m_ID;

public:
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer();

	VertexBuffer(const VertexBuffer& buffer) = delete;
	VertexBuffer& operator=(const VertexBuffer& buffer) = delete;

	void bind() const;
	void unbind() const;
	void setSubData(unsigned int offset, size_t dataSize, const void* data) const;
};

