#pragma once

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_ID = 0;

public:
	VertexArray();
	~VertexArray();

	VertexArray(const VertexArray& tex) = delete;
	VertexArray& operator=(const VertexArray& tex) = delete;

	void addBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) const;
	void bind() const;
	void unbind() const;

};

