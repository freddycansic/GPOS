#pragma once

#include "engine/rendering/opengl/VertexArray.h"
#include "engine/rendering/opengl/IndexBuffer.h"
#include "engine/rendering/opengl/Shader.h"
#include "engine/Window.h"

class Renderer
{
private:
	Renderer();

	static Mat4 s_Persp, s_Ortho, s_Proj, s_View;

	static float s_WindowWidth, s_WindowHeight;

public:

	static void init(const Window& window);
	static void clear(float r, float g, float b);
	
	static void draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader);

	static void setProjectionMatrix(const Mat4& mat);
	static void setViewMatrix(const Mat4& mat);
};

