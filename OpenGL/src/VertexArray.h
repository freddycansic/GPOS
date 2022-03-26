#pragma once

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout);
	void bind() const;
	void unbind() const;

private:
	unsigned int m_ID;

};

