#include "Renderer.h"

#include "engine/viewport/Camera.h"
#include "engine/Debug.h"
#include "engine/Stats.h"
#include "engine/Window.h"

Mat4x4 s_Persp;
Mat4x4 s_Ortho;
Mat4x4 s_Proj;
Mat4x4 s_ViewProj;

RenderMode s_RenderMode = RenderMode::SOLID;
ProjectionMode s_ProjectionMode = ProjectionMode::PERSPECTIVE;

namespace Renderer
{
	void init() {
		GLAPI(glEnable(GL_BLEND)); // enable alpha blending
		GLAPI(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		//glBlendEquation(GL_FUNC_ADD); default
		
		GLAPI(glEnable(GL_DEPTH_TEST));

		recalculateProjectionMatrices();

		// use persp projection by default
		s_Proj = s_Persp;
	}

	void draw(const VertexArray& vao, const IndexBuffer& ibo, Shader& shader) {
		s_ViewProj = s_Proj * Camera::getViewMatrix();

		shader.bind();
		shader.setUniformMat4("u_ViewProj", s_ViewProj);

		vao.bind();
		ibo.bind();

		GLAPI(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ibo.getCount()), ibo.getType(), nullptr));

		++Stats::drawCallsPerFrame;
	}

	void clear(float r, float g, float b)
	{
		GLAPI(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
		GLAPI(glClearColor(r, g, b, 1.0f));
	}

	void clear(const Vec3& colour)
	{
		clear(colour.r, colour.g, colour.b);
	}

	void recalculateProjectionMatrices()
	{
		s_Persp = Maths::perspective(Maths::radians(45.0f), static_cast<float>(Window::displayWidth()) / static_cast<float>(Window::displayHeight()), 1.0f, 1000.0f);
		s_Ortho = Maths::ortho(static_cast<float>(-Window::width()) / 2, static_cast<float>(Window::width()) / 2, static_cast<float>(-Window::height()) / 2, static_cast<float>(Window::height()) / 2, 0.1f, 100.0f);
	}

	void setRenderMode(RenderMode renderMode)
	{
		s_RenderMode = renderMode;

		switch (renderMode)
		{
		case RenderMode::WIREFRAME:
			GLAPI(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
			break;

		case RenderMode::SOLID:
			GLAPI(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
			break;
		}
	}

	RenderMode getRenderMode()
	{
		return s_RenderMode;
	}

	void setProjectionMode(ProjectionMode projectionMode)
	{
		s_ProjectionMode = projectionMode;

		switch (projectionMode)
		{
			case ProjectionMode::PERSPECTIVE:
				s_Proj = s_Persp;
				break;

			case ProjectionMode::ORTHOGRAPHIC:
				s_Proj = s_Ortho;
				break;
		}
	}

	void setProjectionMatrix(const Mat4x4& mat) {
		s_Proj = mat;
	}

	const Mat4x4& getProjectionMatrix()
	{
		return s_Proj;
	}

	const Mat4x4& getViewProjectionMatrix()
	{
		return s_ViewProj;
	}
}