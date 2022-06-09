#include "Application.h"

#include "imgui/imgui.h"

#include "engine/Files.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ShapeRenderer.h"

void Application::init() {
	ShapeRenderer::init();

	tex1 = std::make_unique<Texture>(Files::internal("textures/image.png"));
	tex2 = std::make_unique<Texture>(Files::internal("textures/hashinshin.png"));

	cube1 = Cube(0, 0, 0, 10.0f);
	cube2 = Cube(-5, 5, 0, 10.0f);
	cube3 = Cube(-8, -5, 0, 2);

	rect1 = Rectangle(5, 5, 5, 5);
	rect2 = Rectangle(8, 9, 3, 10);
}

void Application::render() {
	Renderer::clear(0.42f, 0.42f, 0.42f);

	const float radius = 10.0f;
	const float camX = sin(Window::getCurrentTime()) * radius;
	const float camZ = cos(Window::getCurrentTime()) * radius;
	Mat4 view = Mat4::lookAt(Vec3(camX, 0.0, camZ), Vec3(0.0, 0.0, 0.0), Vec3(0.0, 1.0, 0.0));

	//Renderer::setViewMatrix(Mat4::identity.rotate(-viewTransform.rot.x, -viewTransform.rot.y, viewTransform.rot.z).translate(viewTransform.tra.x, viewTransform.tra.y, viewTransform.tra.z).scale(viewTransform.sca.x, viewTransform.sca.y, viewTransform.sca.z));

	Renderer::setViewMatrix(view);

	cube1.setTranslation(cubeTransform.tra.x, cubeTransform.tra.y, cubeTransform.tra.z);
	cube1.setRotation(cubeTransform.rot.x, Window::getCurrentTime() * 50, Window::getCurrentTime() * 35);
	cube1.setScale(cubeTransform.sca.x, cubeTransform.sca.y, cubeTransform.sca.z);

	ShapeRenderer::begin();

	ShapeRenderer::draw(cube1, *tex1);
	ShapeRenderer::draw(cube2, *tex2);
	ShapeRenderer::draw(cube3, {0, 1, 0, 1});
	ShapeRenderer::draw(rect1, {1, 1, 0, 1});

	ShapeRenderer::end();
}

void Application::imGuiRender() {
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::Begin("Debug", reinterpret_cast<bool*>(1), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
	
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

	ImGui::Text("%.1f FPS", static_cast<double>(ImGui::GetIO().Framerate));
	
	ImGui::End();
}

void Application::destroy() {

}