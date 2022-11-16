#include "Renderer.h"

#include "engine/viewport/Camera.h"
#include "engine/Debug.h"
#include "engine/Window.h"

// init matrices
Mat4x4 Renderer::s_Persp;
Mat4x4 Renderer::s_Ortho;
Mat4x4 Renderer::s_Proj;
Mat4x4 Renderer::s_ViewProj;

void Renderer::init() {
	GLAPI(glEnable(GL_BLEND)); // enable alpha blending
	GLAPI(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	//glBlendEquation(GL_FUNC_ADD); default
	
	GLAPI(glEnable(GL_DEPTH_TEST));

	recalculateProjectionMatrices();

	// use persp projection by default
	s_Proj = s_Persp;
}

void Renderer::draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) {
	s_ViewProj = s_Proj * Camera::getViewMatrix();

	shader.bind();
	shader.setUniformMat4("u_ViewProj", s_ViewProj);

	vao.bind();
	ibo.bind();

	GLAPI(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ibo.getCount()), ibo.getType(), nullptr));
}

void Renderer::clear(float r, float g, float b)
{
	GLAPI(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	GLAPI(glClearColor(r, g, b, 1.0f));
}

void Renderer::clear(const Vec3& colour)
{
	clear(colour.r, colour.g, colour.b);
}

void Renderer::setProjectionMatrix(const Mat4x4& mat) {
	s_Proj = mat;
}

const Mat4x4& Renderer::getProjectionMatrix()
{
	return s_Proj;
}

const Mat4x4& Renderer::getViewProjectionMatrix()
{
	return s_ViewProj;
}

void Renderer::recalculateProjectionMatrices()
{
	s_Persp = Maths::perspective(Maths::radians(45.0f), static_cast<float>(Window::displayWidth()) / static_cast<float>(Window::displayHeight()), 1.0f, 1000.0f);
	s_Ortho = Maths::ortho(static_cast<float>(-Window::width()) / 2, static_cast<float>(Window::width()) / 2, static_cast<float>(-Window::height()) / 2, static_cast<float>(Window::height()) / 2, -1.0f, 1.0f);
}

void Renderer::setRenderMode(RenderMode renderMode)
{
	switch (renderMode)
	{
	case RenderMode::Wireframe:
		GLAPI(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		break;

	case RenderMode::Solid:
		GLAPI(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		break;
	}
}