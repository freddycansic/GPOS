#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Window.h"
#include "maths/Mat4.h"
#include "maths/Vectors.h"

struct Vertex {
	static constexpr size_t length = 4;
	Vec2<GLfloat> position;
	Vec2<GLfloat> textureCoordinates;
};

int main(void)
{
	//Mat4 A(
	//	1, 2, 3, 4,
	//	5, 6, 7, 8,
	//	9, 10, 11, 12,
	//	13, 14, 15, 16
	//);

	//Mat4 B(
	//	2, 4, 6, 8,
	//	10, 12, 14, 16,
	//	18, 20, 22, 24,
	//	26, 28, 30, 32
	//);

	Window window(3*1920/4, 3*1080/4, "Hello, world!");

	std::vector<Vertex> vertices = {
		{{ 0.5f,  0.5f}, {1.0f, 1.0f}},
		{{-0.5f,  0.5f}, {0.0f, 1.0f}},
		{{-0.5f, -0.5f}, {0.0f, 0.0f}},
		{{ 0.5f, -0.5f}, {1.0f, 0.0f}},
	};

	const size_t VERTEX_BUFFER_LENGTH = vertices.size() * Vertex::length; // 4 vertices total

	std::vector<GLuint> indices = {
		0, 1, 2,
		0, 2, 3
	};

	VertexArray vao;

	VertexBuffer vbo(&vertices[0], VERTEX_BUFFER_LENGTH * sizeof(GLfloat));

	IndexBuffer ibo(indices.data(), GL_UNSIGNED_INT, indices.size());

	VertexBufferLayout layout;
	layout.addElement<GLfloat>(2, false);
	layout.addElement<GLfloat>(2, false);

	vao.addBuffer(vbo, layout);
	vao.bind();
	vbo.bind();
	ibo.bind();

	Shader shader("res/shaders/default.vert", "res/shaders/default.frag");
	shader.bind();

	Texture texture("res/textures/kali.png");
	texture.bind(); // default no params = texture slot 0

	// texture is bound to slot 0
	shader.setUniform1i("u_Texture", 0);
	
	Renderer r;

	// unbind vao before ibo
	vao.unbind();
	vbo.unbind();
	shader.unbind();
	// so unbinding the ibo doesn't affect the vao
	ibo.unbind();

	float xTranslate = 0.0f, increment = 0.05f;

	while (!window.shouldClose()) {
		float delta = window.getDelta();

		r.clear();
		
		if (xTranslate > 8 || xTranslate < -8) increment *= -1;
		xTranslate += increment * delta * 50;

		// translation, rotation, scale function = scale, rotate, translate matrix
		Mat4 model = Mat4::identity().translate(0.0f, xTranslate, 0.0f).rotate(0, 0, Window::getCurrentTime()).scale(2.0f);

		Mat4 view = Mat4::identity();

		Mat4 proj = Mat4::ortho(-8, 8, -4.5f, 4.5f);

		Mat4 mvp = proj * view * model;

		shader.bind();
		shader.setUniformMat4("u_ModelViewProj", mvp);

		// draw
		vao.bind();
		r.draw(vao, ibo, shader);

		window.update();
	}

	return 0;
}