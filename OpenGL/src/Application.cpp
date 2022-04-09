#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "engine/VertexBuffer.h"
#include "engine/IndexBuffer.h"
#include "engine/VertexArray.h"
#include "engine/VertexBufferLayout.h"
#include "engine/Shader.h"
#include "engine/Renderer.h"
#include "engine/Texture.h"
#include "engine/Window.h"
#include "engine/Vertex.h"
#include "engine/ShapeRenderer.h"
#include "engine/Files.h"

#include "maths/Mat4.h"
#include "maths/Vectors.h"

#include "shapes/Rectangle.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
	Window window(3 * 1920 / 4, 3 * 1080 / 4, "Hello, world!");
	
	Rectangle rect(window.getWidth()/2, window.getHeight()/2, 100, 100);
	rect.setTexID(1);

	//std::vector<Vertex> vertices = {
	//	{{-6.0f,  0.5f, 0.0f}, {1.0f, 1.0f}, 0.0f},
	//	{{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}, 0.0f},
	//	{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, 0.0f},
	//	{{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, 0.0f},

	//	{{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}, 1.0f},
	//	{{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}, 1.0f},
	//	{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, 1.0f},
	//	{{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, 1.0f},
	//};

	auto vertices = rect.vertices;

	std::vector<GLuint> indices = {
		0, 1, 2,
		0, 2, 3,
	};

	VertexArray vao;

	VertexBuffer vbo(vertices.data(), vertices.size() * sizeof(Vertex));

	IndexBuffer ibo(indices.data(), GL_UNSIGNED_INT, indices.size());
	
	VertexBufferLayout layout;
	layout.addElement<GLfloat>(3, false);
	layout.addElement<GLfloat>(4, false);
	layout.addElement<GLfloat>(2, false);
	layout.addElement<GLfloat>(1, false);

	vao.addBuffer(vbo, layout);
	vao.bind();
	vbo.bind();
	ibo.bind();

	Shader shader(Files::internal("shaders/default.vert"), Files::internal("shaders/default.frag"));
	
	Renderer::init();
	ShapeRenderer::init();

	Texture kaliTex(Files::internal("textures/kali.png"));
	Texture elliotTex(Files::internal("textures/image.png"));

	std::array<int, 2> slots = {0, 1};
	shader.setUniform1iv("u_Textures", slots.size(), slots.data());
	

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

	Mat4 proj = Mat4::ortho(0, window.getWidth(), 0, window.getHeight());
	
	float xTranslate = 0.0f, yTranslate = 0.0f;

	while (!window.shouldClose()) {
		float delta = window.getDelta();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		Renderer::clear(0.62f, 0.62f, 0.62f);

		// translation, rotation, scale function = scale, rotate, translate matrix
		Mat4 model = Mat4::identity.translate(xTranslate, yTranslate, 0.0f);//.rotate(0, 0, Window::getCurrentTime());
		Mat4 view = Mat4::identity;
		Mat4 mvp = proj * view * model;
		
		// push mvp uniform to shader
		shader.bind();
		shader.setUniformMat4("u_ModelViewProj", mvp);
		
		// bind textures
		kaliTex.bindToSlot(0);
		elliotTex.bindToSlot(1);

		// draw
		Renderer::draw(vao, ibo, shader);

		{
			ImGui::SetNextWindowPos(ImVec2(10, 10));
			ImGui::Begin("Debug", (bool*)1, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
			
			ImGui::SliderFloat("X", &xTranslate, window.getWidth() / -2, window.getWidth() / 2);
			ImGui::SliderFloat("Y", &yTranslate, window.getHeight() / -2, window.getHeight() / 2);
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