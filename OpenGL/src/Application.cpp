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
	static Line x({ -100, 0, 0 }, { 100, 0, 0 }, 0.01f);
	static Line y({ 0, -100, 0 }, { 0, 100, 0 }, 0.01f);
	static Line z({ 0, 0, -100 }, { 0, 0, 100 }, 0.01f);
	ShapeRenderer::draw(x, { 1, 0, 0, 1 }); // X
	ShapeRenderer::draw(y, { 0, 1, 0, 1 }); // Y
	ShapeRenderer::draw(z, { 0, 0, 1, 1 }); // Z
}

void Application::init(char* projectDir)
{
	openedProject = projectDir;
	std::cout << "Project " << (openedProject == nullptr ? "NO_PROJECT" : openedProject) << " loaded." << std::endl;

	ShapeRenderer::init();

	tex1 = Texture(Files::internal("textures/image.png"));
	tex2 = Texture(Files::internal("textures/hashinshin.png"));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Window::beginCursorCapture();

	constexpr float numCubesSide = 5;
	constexpr float numCubesSideHalf = numCubesSide / 2.0f;

	for (float x = -numCubesSideHalf; x < numCubesSideHalf; ++x)
	{
		for (float y = -numCubesSideHalf; y < numCubesSideHalf; ++y)
		{
			for (float z = -numCubesSideHalf; z < numCubesSideHalf; ++z)
			{
				gameObjects.emplace_back(Cube(x, y, z, 0.5f), Vec4(x/10.0f, y/10.0f, z/10.0f, 1.0f));
			}
		}
	}
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

	for (unsigned int i = 0; i < gameObjects.size() / 2; ++i)
	{
		auto& cube = gameObjects.at(i).first;
		const auto& colour = gameObjects.at(i).second;

		cube.setRotation(Window::currentTime() * 50, Window::currentTime() * 50, 0);
		ShapeRenderer::draw(cube, colour);
	} 

	for (unsigned int i = gameObjects.size() / 2; i < gameObjects.size(); ++i)
	{
		auto& cube = gameObjects.at(i).first;
		const auto& colour = gameObjects.at(i).second;

		ShapeRenderer::draw(cube, colour);
	}
	
	drawAxes();

	ShapeRenderer::end();
}

float colour[4];

void Application::imGuiRender()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::Begin("Debug", reinterpret_cast<bool*>(1), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("%.1f FPS", static_cast<double>(ImGui::GetIO().Framerate));

	ImGui::Text("Objects");

	//if (ImGui::Button("Cube"))
	//{
	//	gameObjects.emplace_back(std::make_pair(std::make_unique<Cube>(0, 0, 0, 0.1f), Vec4(colour[0], colour[1], colour[2], 255)));
	//}

	//ImGui::SameLine();
	//ImGui::Text("%i", gameObjects.size());

	//ImGui::ColorPicker4("##picker", colour, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB);

	//if (!gameObjects.empty())
	//{
	//	const auto& lastShape = gameObjects.at(gameObjects.size() - 1).first;

	//	ImGui::Text("Transform");

	//	// TODO
	//	ImGui::SliderFloat3("Translation", lastShape->translationPtr(), -10, 10); // gross writes directly into memory
	//	ImGui::SliderFloat3("Rotation", lastShape->rotationPtr(), 0, 360); // gross writes directly into memory
	//	ImGui::SliderFloat3("Scale", lastShape->scalePtr(), 0, 10); // gross writes directly into memory
	//}

	ImGui::End();
}

void Application::destroy()
{

}