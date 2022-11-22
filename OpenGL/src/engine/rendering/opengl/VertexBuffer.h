#pragma once

#include <GLEW/glew.h>

class VertexBuffer
{
private:
	unsigned int m_ID = 0;

public:
	VertexBuffer(const void* data, size_t size);
	~VertexBuffer();

	VertexBuffer(const VertexBuffer& vbo) = delete;
	VertexBuffer& operator=(const VertexBuffer& vbo) = delete;

	void bind() const;
	void unbind() const;
	void setSubData(unsigned int offset, size_t dataSize, const void* data) const;
	void setData(size_t dataSizeBytes, const void* data) const;
};

