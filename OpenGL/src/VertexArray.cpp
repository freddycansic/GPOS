#include "VertexArray.h"

#include <iostream>

VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_ID);
	glBindVertexArray(m_ID);
}

void VertexArray::addBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) {
	this->bind();
	buffer.bind();
	
	// for each element in the layout add it
	unsigned int index = 0, offset = 0;
	for (const auto& element : layout.getElements()) {
		std::cout << "index = " << index << " count = " << element.count << " normalised = " << element.normalised << " stride = " << layout.getStride() << " offset = " << offset << std::endl;
	
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index++, element.count, element.type, element.normalised ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)offset);
		
		offset += element.size;
	}
}

void VertexArray::bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_ID);
}