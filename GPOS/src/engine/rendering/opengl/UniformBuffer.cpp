#include "UniformBuffer.h"

#include "engine/Debug.h"

UniformBuffer::UniformBuffer(const void* data, size_t dataSizeBytes)
{
	GLAPI(glGenBuffers(1, &m_ID));
	GLAPI(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
	GLAPI(glBufferData(GL_UNIFORM_BUFFER, dataSizeBytes, data, GL_DYNAMIC_DRAW));
}

UniformBuffer::~UniformBuffer()
{
	GLAPI(glDeleteBuffers(1, &m_ID));
}

void UniformBuffer::bind()
{
	GLAPI(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
}

void UniformBuffer::unbind()
{
	GLAPI(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBuffer::setSubData(unsigned int offset, size_t dataSizeBytes, const void* data) const
{
	GLAPI(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
	GLAPI(glBufferSubData(GL_UNIFORM_BUFFER, offset, dataSizeBytes, data));
}

void UniformBuffer::bindBufferBase(size_t index) const
{
	GLAPI(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
	GLAPI(glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(index), m_ID));
}
