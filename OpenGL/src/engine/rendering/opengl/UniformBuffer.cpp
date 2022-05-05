#include "UniformBuffer.h"

#include "engine/Debug.h"

UniformBuffer::UniformBuffer(const void* data, size_t size)
{
	GLAPI(glGenBuffers(1, &m_ID));
	GLAPI(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
	GLAPI(glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW));
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

void UniformBuffer::setSubData(unsigned int offset, size_t dataSize, const void* data) const
{
	GLAPI(glBindBuffer(GL_UNIFORM_BUFFER, m_ID));
	GLAPI(glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_ID, 0, dataSize));
	GLAPI(glBufferSubData(GL_UNIFORM_BUFFER, 0, dataSize, data));
}
