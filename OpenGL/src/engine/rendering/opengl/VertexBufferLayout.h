#pragma once

#include <GL/glew.h>
#include <iostream>
#include <concepts>
#include <vector>

// i really just wanted to write concept
template<typename T>
concept Numeric = std::integral<T> && std::floating_point<T>;

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
	void addElement(unsigned int count, bool normalised) {
		std::cout << "VertexBufferLayout : Bad element type\n";
		__debugbreak();
	}

	template<>
	void addElement<GLfloat>(unsigned int count, bool normalised) {
		unsigned int size = count * sizeof(GLfloat);

		m_Elements.push_back({GL_FLOAT, count, size, normalised});
		m_Stride += size; // increment stride by new component 
	}

	template<>
	void addElement<GLuint>(unsigned int count, bool normalised) {
		unsigned int size = count * sizeof(GLuint);
		
		m_Elements.push_back({ GL_UNSIGNED_INT, count, size, normalised});
		m_Stride += size;
	}
	
	template<>
	void addElement<uint64_t>(unsigned int count, bool normalised) {
		unsigned int size = count * sizeof(uint64_t);

		m_Elements.push_back({ GL_UNSIGNED_INT64_VEC2_ARB, count, size, normalised });
		m_Stride += size;
	}

	// rlly small uints
	template<>
	void addElement<GLbyte>(unsigned int count, bool normalised) {
		unsigned int size = count * sizeof(GLbyte);

		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, size, true});
		m_Stride += size;
	}

	const std::vector<VertexBufferElement>& getElements() const { return m_Elements; }
	unsigned int getStride() const { return m_Stride; }
};

