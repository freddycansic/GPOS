#pragma once

#include "engine/rendering/opengl/VertexArray.h"
#include "engine/rendering/opengl/IndexBuffer.h"
#include "engine/rendering/opengl/Shader.h"
#include "maths/Maths.h"

enum class RenderMode
{
	SOLID,
	WIREFRAME
};

namespace Renderer
{
	void init();

	void clear(float r, float g, float b);
	void clear(const Vec3& colour);
	
	void draw(const VertexArray& vao, const IndexBuffer& ibo, Shader& shader);
 	
 	void recalculateProjectionMatrices();
 	const Mat4x4& getProjectionMatrix();
	void setProjectionMatrix(const Mat4x4& mat);
	
	const Mat4x4& getViewProjectionMatrix();
 	
	RenderMode getRenderMode();
	void setRenderMode(RenderMode renderMode);
}