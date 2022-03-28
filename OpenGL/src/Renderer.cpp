#include "Renderer.h"

Renderer::Renderer() {
	glEnable(GL_BLEND); // enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendEquation(GL_FUNC_ADD); default
}

void Renderer::draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const {

	shader.bind();
	vao.bind();
	ibo.bind();

	glDrawElements(GL_TRIANGLES, ibo.getCount(), ibo.getType(), nullptr);

}

void Renderer::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}
