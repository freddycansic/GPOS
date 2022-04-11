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

	Renderer::init();
	ShapeRenderer::init();

	// imgui
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window.getGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();

	Mat4 proj = Mat4::ortho(0, window.getWidth(), 0, window.getHeight());
	
	float xTranslate = 0.0f, yTranslate = 0.0f;

	while (!window.shouldClose()) {
		float delta = window.getDelta();
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		Renderer::clear(0.42f, 0.42f, 0.42f);

		// translation, rotation, scale function = scale, rotate, translate matrix
		Mat4 model = Mat4::identity.translate(xTranslate, yTranslate, 0.0f);//.rotate(0, 0, Window::getCurrentTime());
		//Mat4 model = Mat4::identity;//.rotate(0, 0, Window::getCurrentTime());
		Mat4 view = Mat4::identity;
		Mat4 mvp = proj * view * model;
		
		// push mvp uniform to shader // TODO TEMPORARY
		ShapeRenderer::s_Shader->bind();
		ShapeRenderer::s_Shader->setUniformMat4("u_ModelViewProj", mvp);
		
		ShapeRenderer::begin();
		ShapeRenderer::draw(rect, {0.3f, 0.1f, 1.0f, 1.0f});
		ShapeRenderer::draw(Rectangle(10.0f, 10.0f, 100.0f, 100.0f), {0.0f, 1.0f, 1.0f, 1.0f});
		ShapeRenderer::end();

		{
			ImGui::SetNextWindowPos(ImVec2(10, 10));
			ImGui::Begin("Debug", (bool*)1, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
			
			ImGui::SliderFloat("X", &xTranslate, window.getWidth() / -2, window.getWidth() / 2);
			ImGui::SliderFloat("Y", &yTranslate, window.getHeight() / -2, window.getHeight() / 2);
			ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
			
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