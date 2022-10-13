#include "VertexBuffer.h"

#include "engine/Debug.h"

VertexBuffer::VertexBuffer(const void* data, size_t size) {
	GLAPI(glGenBuffers(1, &m_ID));
	GLAPI(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
	GLAPI(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

void VertexBuffer::bind() const {
	GLAPI(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
}

void VertexBuffer::unbind() const {
	GLAPI(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::setSubData(unsigned int offset, size_t dataSize, const void* data) const
{
	GLAPI(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
	GLAPI(glBufferSubData(GL_ARRAY_BUFFER, offset, dataSize, data));
}

void VertexBuffer::setData(size_t dataSizeBytes, const void* data) const
{
	GLAPI(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
	GLAPI(glBufferData(GL_ARRAY_BUFFER, dataSizeBytes, data, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
	GLAPI(glDeleteBuffers(1, &m_ID));
}