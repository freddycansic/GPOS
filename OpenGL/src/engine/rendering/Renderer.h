#pragma once

#include "engine/rendering/opengl/VertexArray.h"
#include "engine/rendering/opengl/IndexBuffer.h"
#include "engine/rendering/opengl/Shader.h"

class Renderer
{
private:
	Renderer();

public:
	static void init();
	static void clear(float r, float g, float b);
	
	static void draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader);

};

