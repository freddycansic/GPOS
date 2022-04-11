#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, size_t size) {
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer() :
	m_ID(0)
{
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setSubData(unsigned int offset, size_t dataSize, const void* data) const
{
	glBufferSubData(GL_ARRAY_BUFFER, offset, dataSize, data);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &m_ID);
}