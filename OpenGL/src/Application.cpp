#include "Application.h"

#include "imgui/imgui.h"

#include "engine/Files.h"
#include "engine/Input.h"
#include "engine/Keys.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/Window.h"
#include "engine/rendering/shapes/Line.h"

void drawAxes()
{
	const static Line x({ -100, 0, 0 }, { 100, 0, 0 }, 0.01f);
	const static Line y({ 0, -100, 0 }, { 0, 100, 0 }, 0.01f);
	const static Line z({ 0, 0, -100 }, { 0, 0, 100 }, 0.01f);
	ShapeRenderer::draw(x, { 1, 0, 0, 1 }); // X
	ShapeRenderer::draw(y, { 0, 1, 0, 1 }); // Y
	ShapeRenderer::draw(z, { 0, 0, 1, 1 }); // Z
}

void Application::init()
{
	ShapeRenderer::init();

	tex1 = Texture(Files::internal("textures/image.png"));
	tex2 = Texture(Files::internal("textures/hashinshin.png"));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Window::beginCursorCapture();
}

Vec3 cameraPos = { 0.0f, 0.0f, 30.0f };
Vec3 cameraFront = { 0.0f, 0.0f, -1.0f }; // point forward
Vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
Vec3 cameraOrbit, cameraTarget;

void Application::render()
{
	Renderer::clear(0.42f, 0.42f, 0.42f);
	
	cameraFront = Input::getCameraDirection();

	// camera position movement
	const float moveSpeed = 10.0f * (Input::isKeyDown(Keys::LEFT_SHIFT) ? 2.0f : 1.0f);

	//if (Input::isKeyDown(Keys::W)) {
	if (Input::isKeyDown(Keys::W)) {
		cameraPos += cameraFront * moveSpeed * Window::deltatime();
	}
	if (Input::isKeyDown(Keys::S)) {
		cameraPos -= cameraFront * moveSpeed * Window::deltatime();
	}
	if (Input::isKeyDown(Keys::D)) {
		cameraPos -= cameraFront.cross(cameraUp).normalise() * moveSpeed * Window::deltatime();
	}
	if (Input::isKeyDown(Keys::A)) {
		cameraPos += cameraFront.cross(cameraUp).normalise() * moveSpeed * Window::deltatime();
	}

	const Mat4 view = Mat4::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	Renderer::setViewMatrix(view);

	if (Input::isKeyDown(Keys::V))
	{
		Window::endCursorCapture();
	}

	if (Input::isKeyDown(Keys::C))
	{
		Window::beginCursorCapture();
	}

	ShapeRenderer::begin();

	for (const auto& [shape, colour] : gameObjects)
	{
		ShapeRenderer::draw(*shape, colour);
	}

	drawAxes();

	ShapeRenderer::draw(Cube(0, 0, 0, 2), tex2);

	ShapeRenderer::end();
}

float colour[4];

void Application::imGuiRender()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::Begin("Debug", reinterpret_cast<bool*>(1), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("%.1f FPS", static_cast<double>(ImGui::GetIO().Framerate));

	ImGui::Text("Objects");

	if (ImGui::Button("Cube"))
	{
		gameObjects.emplace_back(std::make_pair(std::make_unique<Cube>(0, 0, 0, 0.1f), Vec4(colour[0], colour[1], colour[2], 255)));
	}

	ImGui::SameLine();
	ImGui::Text("%i", gameObjects.size());

	ImGui::ColorPicker4("##picker", colour, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB);

	if (!gameObjects.empty())
	{
		const auto& lastShape = gameObjects.at(gameObjects.size() - 1).first;

		ImGui::Text("Transform");

		// TODO
		ImGui::SliderFloat3("Translation", lastShape->translationPtr(), -10, 10); // gross writes directly into memory
		ImGui::SliderFloat3("Rotation", lastShape->rotationPtr(), 0, 360); // gross writes directly into memory
		ImGui::SliderFloat3("Scale", lastShape->scalePtr(), 0, 10); // gross writes directly into memory
	}

	ImGui::End();
}

void Application::destroy()
{

}