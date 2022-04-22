#include "Application.h"
#include <engine/rendering/shapes/Cube.h>

void Application::init() {
	ShapeRenderer::init();

	Vec3 pos(1.0f, 0.0f, 0.0f);
	std::cout << Mat4::identity.rotate(90.0f, 0.0f, 0.0f) * Vec4(pos, 1.0f) << std::endl;
	
	tex1 = std::make_unique<Texture>(Files::internal("textures/kali.png"));

	cube1 = Cube(0, 0, 0, 10.0f);
}

void Application::render() {
	Renderer::clear(0.42f, 0.42f, 0.42f);

	Renderer::setViewMatrix(Mat4::identity.translate(viewTransform.tra.x, viewTransform.tra.y, viewTransform.tra.z));

	//cube1.setRotation(0, 0, 0);
	cube1.setRotation(cubeTransform.rot.x, cubeTransform.rot.y, cubeTransform.rot.z);
	cube1.setTranslation(cubeTransform.tra.x, cubeTransform.tra.y, cubeTransform.tra.z);
	cube1.setScale(cubeTransform.sca.x, cubeTransform.sca.y, cubeTransform.sca.z);

	ShapeRenderer::begin();
	ShapeRenderer::draw(cube1, {1, 1, 0, 1});
	ShapeRenderer::end();
}

void Application::imGuiRender() {
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::Begin("Debug", (bool*)1, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
	
	ImGui::SliderFloat("X", &xTranslate, windowWidth / -2, windowWidth / 2);
	ImGui::SliderFloat("Y", &yTranslate, windowHeight / -2, windowHeight / 2);
	ImGui::SliderFloat("Z", &zTranslate, -1000.0f, 1000.0f);
	ImGui::SliderFloat3("Z", &zTranslate, -1000.0f, 1000.0f);
	
	ImGui::SliderFloat("X View", &xViewTranslate, windowWidth / -2, windowWidth / 2);
	ImGui::SliderFloat("Y View", &yViewTranslate, windowHeight / -2, windowHeight / 2);
	ImGui::SliderFloat("Z View", &zViewTranslate, -1000.0f, 1000.0f);


	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	
	ImGui::End();
}

void Application::destroy() {

}