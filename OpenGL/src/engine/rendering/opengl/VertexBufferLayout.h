#pragma once

#include <GL/glew.h>
#include <iostream>
#include <vector>

// each layout component contains these 3
struct VertexBufferElement {
	GLenum type;
	unsigned int count;
	unsigned int size;
	bool normalised;
};

class VertexBufferLayout
{
private:
	unsigned int m_Stride; // stride = total size of each vertex in bytes
	std::vector<VertexBufferElement> m_Elements; // contents of the layout

public:

	VertexBufferLayout() : m_Stride(0) {}
	~VertexBufferLayout() { m_Elements.clear(); m_Elements.shrink_to_fit(); }

	template<typename T>
	void addElement(unsigned int count, bool normalised);

	const std::vector<VertexBufferElement>& getElements() const { return m_Elements; }
	unsigned int getStride() const { return m_Stride; }
};

