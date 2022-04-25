#include "Application.h"
#include <engine/rendering/shapes/Cube.h>

void Application::init() {
	ShapeRenderer::init();

	tex1 = std::make_unique<Texture>(Files::internal("textures/kali.png"));

	cube1 = Cube(0, 0, 0, 10.0f);
}

void Application::render() {
	Renderer::clear(0.42f, 0.42f, 0.42f);

	Renderer::setViewMatrix(Mat4::identity.translate(viewTransform.tra.x, viewTransform.tra.y, viewTransform.tra.z).rotate(viewTransform.rot.x, viewTransform.rot.y, viewTransform.rot.z).scale(viewTransform.sca.x, viewTransform.sca.y, viewTransform.sca.z));

	cube1.setTranslation(cubeTransform.tra.x, cubeTransform.tra.y, cubeTransform.tra.z);
	cube1.setRotation(cubeTransform.rot.x, cubeTransform.rot.y, cubeTransform.rot.z);
	cube1.setScale(cubeTransform.sca.x, cubeTransform.sca.y, cubeTransform.sca.z);

	ShapeRenderer::begin();
	ShapeRenderer::draw(cube1, {1, 1, 0, 1});
	ShapeRenderer::end();
}

void Application::imGuiRender() {
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::Begin("Debug", (bool*)1, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
	
	ImGui::Text("Cube");

	ImGui::Text("Translation");
	ImGui::SliderFloat("##1", &cubeTransform.tra.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##2", &cubeTransform.tra.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##3", &cubeTransform.tra.z, -100.0f, 100.0f);

	ImGui::Text("Rotation");
	ImGui::SliderFloat("##4", &cubeTransform.rot.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##5", &cubeTransform.rot.z, -100.0f, 100.0f);
	ImGui::SliderFloat("##6", &cubeTransform.rot.x, -100.0f, 100.0f);
	
	ImGui::Text("Scale");
	ImGui::SliderFloat("##7", &cubeTransform.sca.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##8", &cubeTransform.sca.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##9", &cubeTransform.sca.z, -100.0f, 100.0f);

	ImGui::Text("View");
	
	ImGui::Text("Translation");
	ImGui::SliderFloat("##10", &viewTransform.tra.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##11", &viewTransform.tra.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##12", &viewTransform.tra.z, -100.0f, 100.0f);
							
	ImGui::Text("Rotation");
	ImGui::SliderFloat("##13", &viewTransform.rot.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##14", &viewTransform.rot.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##15", &viewTransform.rot.z, -100.0f, 100.0f);
							
	ImGui::Text("Scale");	
	ImGui::SliderFloat("##16", &viewTransform.sca.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##17", &viewTransform.sca.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##18", &viewTransform.sca.z, -100.0f, 100.0f);

	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	
	ImGui::End();
}

void Application::destroy() {

}