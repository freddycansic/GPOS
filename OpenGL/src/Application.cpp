#include "Application.h"


void Application::init() {
	ShapeRenderer::init();
	
	tex1 = Texture("res/textures/kali.png");
	proj = Mat4::ortho(0, windowWidth, 0, windowHeight); // TODO add perspective matrix
}

void Application::render() {
	Renderer::clear(0.42f, 0.42f, 0.42f);

	// translation, rotation, scale function = scale, rotate, translate matrix
	Mat4 model = Mat4::identity.translate(xTranslate, yTranslate, 0.0f);
	Mat4 view = Mat4::identity; // TODO
	Mat4 mvp = proj * view * model;

	// push mvp uniform to shader // TODO TEMPORARY
	ShapeRenderer::s_Shader->bind();
	ShapeRenderer::s_Shader->setUniformMat4("u_ModelViewProj", mvp);
	
	ShapeRenderer::begin();
	ShapeRenderer::draw(Rectangle(windowWidth/2-100, windowHeight/2-100, 200, 200), tex1);
	ShapeRenderer::end();

	std::cout << "Rendered." << std::endl;
}

void Application::imGuiRender() {

	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::Begin("Debug", (bool*)1, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
				
	ImGui::SliderFloat("X", &xTranslate, windowWidth / -2, windowWidth / 2);
	ImGui::SliderFloat("Y", &yTranslate, windowHeight / -2, windowHeight / 2);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
				
	ImGui::End();
}

void Application::destroy() {

}

