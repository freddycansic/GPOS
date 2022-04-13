#include "VertexArray.h"

#include <iostream>

#include "engine/Debug.h"

VertexArray::VertexArray() {
	GLAPI(glGenVertexArrays(1, &m_ID));
	GLAPI(glBindVertexArray(m_ID));
}

void VertexArray::addBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) const {
	this->bind();
	buffer.bind();
	
	// for each element in the layout add it
	unsigned int index = 0, offset = 0;
	for (const auto& element : layout.getElements()) {
		//std::cout << "index = " << index << " count = " << element.count << " normalised = " << element.normalised << " stride = " << layout.getStride() << " offset = " << offset << std::endl;
	
		GLAPI(glEnableVertexAttribArray(index));
		GLAPI(glVertexAttribPointer(index++, element.count, element.type, element.normalised ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)offset));
		
		offset += element.size;
	}
}

void VertexArray::bind() const
{
	GLAPI(glBindVertexArray(m_ID));
}

void VertexArray::unbind() const
{
	GLAPI(glBindVertexArray(0));
}

VertexArray::~VertexArray() {
	GLAPI(glDeleteVertexArrays(1, &m_ID));
}