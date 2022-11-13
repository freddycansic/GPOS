#include "VertexBufferLayout.h"

#include <iostream>

#include "engine/Debug.h"

template<typename T>
void VertexBufferLayout::addElement(unsigned int count, bool normalised)
{
	ASSERT_WITH_MSG(false, "Bad element type");
}

template<>
void VertexBufferLayout::addElement<float>(unsigned int count, bool normalised)
{
	size_t size = count * sizeof(GLfloat);

	m_Elements.emplace_back(GL_FLOAT, count, size, normalised);
	m_Stride += size; // increment stride by new component 
}

template<>
void VertexBufferLayout::addElement<int>(unsigned int count, bool normalised)
{
	size_t size = count * sizeof(GLint);

	m_Elements.emplace_back(GL_INT, count, size, normalised);
	m_Stride += size;
}

template<>
void VertexBufferLayout::addElement<unsigned long long>(unsigned int count, bool normalised)
{
	size_t size = count * sizeof(GLuint64);

	m_Elements.emplace_back(GL_UNSIGNED_INT64_ARB, count, size, normalised);
	m_Stride += size;
}