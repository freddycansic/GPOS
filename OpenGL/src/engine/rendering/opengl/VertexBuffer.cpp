#include "VertexBuffer.h"

#include "engine/Debug.h"

VertexBuffer::VertexBuffer(const void* data, size_t size) {
	GLAPI(glGenBuffers(1, &m_ID));
	GLAPI(glBindBuffer(GL_ARRAY_BUFFER, m_ID));
	GLAPI(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer() :
	m_ID(0)
{
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

VertexBuffer::~VertexBuffer() {
	GLAPI(glDeleteBuffers(1, &m_ID));
}