#include "IndexBuffer.h"

#include "engine/Debug.h"

IndexBuffer::IndexBuffer(const void* data, GLenum type, size_t count)
	: m_Count(count), m_Type(type) 
{
	GLAPI(glGenBuffers(1, &m_ID));
	GLAPI(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	GLAPI(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_DYNAMIC_DRAW));
}
	
void IndexBuffer::setSubData(unsigned int offset, size_t count, const void* data) const
{
	GLAPI(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
	GLAPI(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, count * sizeof(GLuint), data));
}

void IndexBuffer::bind() const {
	GLAPI(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID));
}

void IndexBuffer::unbind() const {
	GLAPI(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

IndexBuffer::~IndexBuffer() {
	GLAPI(glDeleteBuffers(1, &m_ID));
}