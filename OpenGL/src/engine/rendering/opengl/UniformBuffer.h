#pragma once

#include <GLEW/glew.h>

class UniformBuffer
{
private:
	unsigned int m_ID = 0;

public:
	UniformBuffer(const void* data, size_t dataSizeBytes);
	~UniformBuffer();

	UniformBuffer(const UniformBuffer& ubo) = delete;
	UniformBuffer& operator=(const UniformBuffer& ubo) = delete;

	void bind();
	void unbind();
	void setSubData(unsigned int offset, size_t dataSizeBytes, const void* data) const;
	void bindBufferBase(size_t index) const;
};