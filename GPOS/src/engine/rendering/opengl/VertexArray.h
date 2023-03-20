#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_ID = 0;

public:
	VertexArray();
	~VertexArray();

	VertexArray(const VertexArray& vao) = delete;
	VertexArray& operator=(const VertexArray& vao) = delete;

	VertexArray(VertexArray&& vao) noexcept;
	VertexArray& operator=(VertexArray&& vao) noexcept;

	void addBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) const;
	void bind() const;
	void unbind() const;
};