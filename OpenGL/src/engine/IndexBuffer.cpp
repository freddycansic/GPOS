#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, GLenum type, size_t count)
	: m_Count(count), m_Type(type) 
{
	
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer() :
	m_ID(0), m_Count(0), m_Type(0)
{
}
	
void IndexBuffer::setSubData(unsigned int offset, size_t count, const void* data) const
{
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, count * sizeof(GLuint), data);
}

void IndexBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &m_ID);
}