#pragma once

#include <GL/glew.h>

class VertexArray
{
public:
	VertexArray();

	void addAttrib(unsigned int index, unsigned int count, GLEnum type, bool normalised, unsigned int stride, unsigned int offset);
	void bind() const;
	void unbind() const;

private:
	unsigned int m_ID;

};

