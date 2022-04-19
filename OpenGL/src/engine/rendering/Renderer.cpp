#include "Renderer.h"

#include "engine/Debug.h"

float Renderer::s_WindowWidth = 0.0f, Renderer::s_WindowHeight = 0.0f;

// init matrices
Mat4 Renderer::s_Persp;
Mat4 Renderer::s_Ortho;
Mat4 Renderer::s_Proj;
Mat4 Renderer::s_View;

void Renderer::init(const Window& window) {
	GLAPI(glEnable(GL_BLEND)); // enable alpha blending
	GLAPI(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//glBlendEquation(GL_FUNC_ADD); default

	s_WindowWidth = window.getWidth();
	s_WindowHeight = window.getHeight();

	s_Persp = Mat4::perspective(45.0f, s_WindowWidth / s_WindowHeight, 1.0f, 1000.0f);
	s_Ortho = Mat4::ortho(-s_WindowWidth / 2, s_WindowWidth / 2, -s_WindowHeight / 2, s_WindowHeight / 2, -1.0f, 1.0f);

	// use persp projection by default
	s_Proj = s_Persp;
	s_View = Mat4::identity;
}

void Renderer::draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) {

	// hard coded translation to move scene in front of near clipping plane
	Mat4 vp = s_Proj * s_View.translate(0, 0, -500);

	shader.bind();
	shader.setUniformMat4("u_ViewProj", vp);

	vao.bind();

	GLAPI(glDrawElements(GL_TRIANGLES, (GLsizei) ibo.getCount(), ibo.getType(), nullptr));
}

void Renderer::clear(float r, float g, float b)
{
	GLAPI(glClear(GL_COLOR_BUFFER_BIT));
	GLAPI(glClearColor(r, g, b, 1.0f));
}

void Renderer::setProjectionMatrix(const Mat4& mat) {
	s_Proj = mat;
}

void Renderer::setViewMatrix(const Mat4& mat) {
	s_View = mat;
}