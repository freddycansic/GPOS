#include "VertexArray.h"

#include <iostream>

#include "engine/Debug.h"

VertexArray::VertexArray() {
	GLAPI(glGenVertexArrays(1, &m_ID));
	GLAPI(glBindVertexArray(m_ID));
}

VertexArray::VertexArray(VertexArray&& vao) noexcept :
	m_ID(vao.m_ID)
{
	vao.m_ID = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& vao) noexcept
{
	this->m_ID = vao.m_ID;
	
	vao.m_ID = 0;

	return *this;
}


void VertexArray::addBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) const {
	this->bind();
	buffer.bind();
	
	unsigned int index = 0, offset = 0;
	for (const auto& [type, count, size, normalised] : layout.getElements()) 
	{
		GLAPI(glEnableVertexAttribArray(index));

		if (type == GL_UNSIGNED_INT64_ARB) 
		{
			GLAPI(glVertexAttribLPointer(index++, count, type, static_cast<GLsizei>(layout.getStride()), reinterpret_cast<const void*>(offset)));

		} else 
		{
			GLAPI(glVertexAttribPointer(index++, count, type, normalised ? GL_TRUE : GL_FALSE, static_cast<GLsizei>(layout.getStride()), reinterpret_cast<const void*>(offset)));
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