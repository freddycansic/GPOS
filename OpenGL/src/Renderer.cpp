#include "Renderer.h"

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
