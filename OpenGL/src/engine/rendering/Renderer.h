#pragma once

#include "engine/rendering/opengl/VertexArray.h"
#include "engine/rendering/opengl/IndexBuffer.h"
#include "engine/rendering/opengl/Shader.h"
#include "maths/Maths.h"

enum class RenderMode
{
	Solid,
	Wireframe
};

class Renderer
{
private:

	static Mat4x4 s_Persp, s_Ortho, s_Proj, s_View;

public:
	Renderer() = delete;

	static void init();
	static void clear(float r, float g, float b);
	
	static void draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader);

	static void setProjectionMatrix(const Mat4x4& mat);
	static Mat4x4 getProjectionMatrix();

	static void setRenderMode(RenderMode renderMode);
};