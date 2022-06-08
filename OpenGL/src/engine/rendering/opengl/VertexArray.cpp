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
	
	unsigned int index = 0, offset = 0;
	for (const auto& [type, count, size, normalised] : layout.getElements()) {
		//std::cout << "index = " << index << " count = " << count << " normalised = " << normalised << " stride = " << layout.getStride() << " offset = " << offset << std::endl;
	
		GLAPI(glEnableVertexAttribArray(index));

		if (type == GL_UNSIGNED_INT64_ARB) {
			GLAPI(glVertexAttribLPointer(index++, count, type, layout.getStride(), reinterpret_cast<const void*>(offset)));

		} else {
			GLAPI(glVertexAttribPointer(index++, count, type, normalised ? GL_TRUE : GL_FALSE, layout.getStride(), reinterpret_cast<const void*>(offset)));
		}
		
		offset += size;
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