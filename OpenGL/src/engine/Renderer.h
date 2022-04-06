#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
private:


public:
	Renderer();
	
	void draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
	void clear() const;

};

