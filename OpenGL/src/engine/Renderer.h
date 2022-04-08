#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Color.h"

class Renderer
{
private:
	Renderer();

public:
	static void init();
	static void clear(float r, float g, float b);
	
	static void draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader);

};

