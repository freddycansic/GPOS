#include "VertexBufferLayout.h"

#include <iostream>

template<typename T>
void VertexBufferLayout::addElement(unsigned int count, bool normalised) {
	std::cout << "VertexBufferLayout : Bad element type" << std::endl;
	__debugbreak();
}

template<>
void VertexBufferLayout::addElement<float>(unsigned int count, bool normalised) {
	size_t size = count * sizeof(GLfloat);

	m_Elements.emplace_back(GL_FLOAT, count, size, normalised);
	m_Stride += size; // increment stride by new component 
}

template<>
void VertexBufferLayout::addElement<int>(unsigned int count, bool normalised) {
	size_t size = count * sizeof(GLint);

	m_Elements.emplace_back(GL_INT, count, size, normalised);
	m_Stride += size;
}