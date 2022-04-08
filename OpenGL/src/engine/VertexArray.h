#pragma once

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_ID;

public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) const;
	void bind() const;
	void unbind() const;

};

