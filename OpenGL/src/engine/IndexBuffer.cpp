#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, GLenum type, size_t count)
	: m_Count(count), m_Type(type) 
{
	
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
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