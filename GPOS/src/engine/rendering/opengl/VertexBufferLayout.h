#pragma once

#include <GL/glew.h>
#include <vector>

struct VertexBufferElement {
	GLenum type;
	unsigned int count;
	unsigned int size;
	bool normalised;
};

class VertexBufferLayout
{
private:
	size_t m_Stride = 0; // stride = total size of each vertex in bytes
	std::vector<VertexBufferElement> m_Elements; // contents of the layout

public:

	VertexBufferLayout() = default;
	~VertexBufferLayout() = default;

	template<typename T>
	void addElement(unsigned int count, bool normalised);

	[[nodiscard]] const std::vector<VertexBufferElement>& getElements() const { return m_Elements; }
	[[nodiscard]] size_t getStride() const { return m_Stride; }
};