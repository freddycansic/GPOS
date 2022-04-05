#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Window.h"
#include "Files.h"

#include "maths/Mat4.h"
#include "maths/Vectors.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

struct Vertex {
	static constexpr size_t length = 5;
	Vec2<GLfloat> position;
	Vec2<GLfloat> textureCoordinates;
	GLfloat texID;
};

int main(void)
{
	Window window(3*1920/4, 3*1080/4, "Hello, world!");

	std::vector<Vertex> vertices = {
		{{-6.0f,  0.5f}, {1.0f, 1.0f}, 0.0f},
		{{-0.5f,  0.5f}, {0.0f, 1.0f}, 0.0f},
		{{-0.5f, -0.5f}, {0.0f, 0.0f}, 0.0f},
		{{ 0.5f, -0.5f}, {1.0f, 0.0f}, 0.0f},

		{{ 0.5f,  0.5f}, {1.0f, 1.0f}, 1.0f},
		{{-0.5f,  0.5f}, {0.0f, 1.0f}, 1.0f},
		{{-0.5f, -0.5f}, {0.0f, 0.0f}, 1.0f},
		{{ 0.5f, -0.5f}, {1.0f, 0.0f}, 1.0f},
	};

	const size_t VERTEX_BUFFER_LENGTH = vertices.size() * Vertex::length; // 4 vertices total

	std::vector<GLuint> indices = {
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7
	};

	VertexArray vao;

	VertexBuffer vbo(vertices.data(), VERTEX_BUFFER_LENGTH * sizeof(GLfloat));
	//VertexBuffer vbo(vertices.data(), sizeof(vertices));

	IndexBuffer ibo(indices.data(), GL_UNSIGNED_INT, indices.size());

	VertexBufferLayout layout;
	layout.addElement<GLfloat>(2, false);
	layout.addElement<GLfloat>(2, false);
	layout.addElement<GLfloat>(1, false);

	vao.addBuffer(vbo, layout);
	vao.bind();
	vbo.bind();
	ibo.bind();

	//Shader shader(Files::internal("shaders/default.vert"), Files::internal("shaders/default.frag"));
	Shader shader("res/shaders/default.vert", "res/shaders/default.frag");
	shader.bind();

	Texture kaliTex("res/textures/kali.png");
	Texture elliotTex("res/textures/image.png");

	std::array<int, 2> slots = {0, 1};
	shader.setUniform1iv("u_Textures", slots.size(), slots.data());
	
	Renderer r;

	// imgui
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window.getGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();

	// unbind vao before ibo
	vao.unbind();
	vbo.unbind();
	shader.unbind();
	// so unbinding the ibo doesn't affect the vao
	ibo.unbind();

	Mat4 proj = Mat4::ortho(-8, 8, -4.5f, 4.5f);
	
	float xTranslate = 0.0f, yTranslate = 0.0f;

	while (!window.shouldClose()) {
		float delta = window.getDelta();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		r.clear();

		// translation, rotation, scale function = scale, rotate, translate matrix
		Mat4 model = Mat4::identity().translate(xTranslate, yTranslate, 0.0f).rotate(0, 0, Window::getCurrentTime()).scale(2.0f);
		Mat4 view = Mat4::identity();
		Mat4 mvp = proj * view * model;
		
		// push mvp uniform to shader
		shader.bind();
		shader.setUniformMat4("u_ModelViewProj", mvp);
		
		// bind textures
		kaliTex.bindSlot(0);
		elliotTex.bindSlot(1);

		// draw
		r.draw(vao, ibo, shader);

		{
			ImGui::SetNextWindowPos(ImVec2(10, 10));
			ImGui::Begin("Debug", (bool*)1, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
			
			ImGui::SliderFloat("X", &xTranslate, -8, 8);
			ImGui::SliderFloat("Y", &yTranslate, -4.5, 4.5);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.update();
	}

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	return 0;
}