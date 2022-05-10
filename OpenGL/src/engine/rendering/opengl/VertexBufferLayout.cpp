#include "VertexBufferLayout.h"

template<typename T>
void VertexBufferLayout::addElement(unsigned int count, bool normalised) {
	std::cout << "VertexBufferLayout : Bad element type\n";
	__debugbreak();
}

template<>
void VertexBufferLayout::addElement<GLfloat>(unsigned int count, bool normalised) {
	unsigned int size = count * sizeof(GLfloat);

	m_Elements.push_back({ GL_FLOAT, count, size, normalised });
	m_Stride += size; // increment stride by new component 
}

template<>
void VertexBufferLayout::addElement<GLuint>(unsigned int count, bool normalised) {
	unsigned int size = count * sizeof(GLuint);

	m_Elements.push_back({ GL_UNSIGNED_INT, count, size, normalised });
	m_Stride += size;
}

template<>
void VertexBufferLayout::addElement<uint64_t>(unsigned int count, bool normalised) {
	unsigned int size = count * sizeof(uint64_t);

	m_Elements.push_back({ GL_UNSIGNED_INT64_ARB, count, size, normalised });
	m_Stride += size;
}

template<>
void VertexBufferLayout::addElement<GLbyte>(unsigned int count, bool normalised) {
	unsigned int size = count * sizeof(GLbyte);

	m_Elements.push_back({ GL_UNSIGNED_BYTE, count, size, true });
	m_Stride += size;
}
