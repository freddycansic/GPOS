#include "VertexBufferLayout.h"

#include <iostream>

template<typename T>
void VertexBufferLayout::addElement(unsigned int count, bool normalised) {
	std::cout << "VertexBufferLayout : Bad element type" << std::endl;
	__debugbreak();
}

template<>
void VertexBufferLayout::addElement<GLfloat>(unsigned int count, bool normalised) {
	unsigned int size = count * sizeof(GLfloat);

	m_Elements.push_back({ GL_FLOAT, count, size, normalised });
	m_Stride += size; // increment stride by new component 
}