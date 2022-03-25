#include "VertexArray.h"

VertexArray::VertexArray() {

}

void VertexArray::addAttrib(unsigned int index, unsigned int count, GLenum type, bool normalised, unsigned int stride, unsigned int offset) {

	glEnableVertexAttribArray(index);
	
}

void VertexArray::bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}
