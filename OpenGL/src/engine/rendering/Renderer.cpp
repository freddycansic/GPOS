#include "Renderer.h"

void Renderer::init() {
	glEnable(GL_BLEND); // enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendEquation(GL_FUNC_ADD); default
}

void Renderer::draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) {

	shader.bind();
	vao.bind();

	glDrawElements(GL_TRIANGLES, (GLsizei) ibo.getCount(), ibo.getType(), nullptr);

}

void Renderer::clear(float r, float g, float b)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(r, g, b, 1.0f);
}