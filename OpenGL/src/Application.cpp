#include "Application.h"

#include "engine/GUI.h"

#include "engine/input/Files.h"
#include "engine/input/Input.h"
#include "engine/input/Keys.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/Window.h"
#include "engine/rendering/shapes/Line.h"
#include "engine/input/Keybind.h"
#include "engine/Camera.h"

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

	Window::beginCursorCapture();

	//constexpr float numCubesSide = 3;
	//constexpr float numCubesSideHalf = numCubesSide / 2.0f;

	//for (float x = -numCubesSideHalf; x < numCubesSideHalf; ++x)
	//{
	//	for (float y = -numCubesSideHalf; y < numCubesSideHalf; ++y)
	//	{
	//		for (float z = -numCubesSideHalf; z < numCubesSideHalf; ++z)
	//		{
	//			//gameObjects.emplace_back(Cube(x, y, z, 0.5f), Vec4(1, 1, 0, 1.0f));
	//			gameObjects.emplace_back(Cube(x, y, z, 0.5f), Vec4(1, 1, 1, 1));
	//		}
	//	}
	//}

	//gameObjects.at(Maths::randint(0, gameObjects.size())).first.setSelected(true);
}

Vec3 cameraPos = { 0.0f, 0.0f, 30.0f };
Vec3 cameraFront = { 0.0f, 0.0f, 1.0f }; // point forward
Vec3 cameraUp = { 0.0f, 1.0f, 0.0f };

void Application::render()
{
	Renderer::clear(0.42f, 0.42f, 0.42f);

	if (Window::capturingCursor())
	{
		cameraFront = Input::getCameraDirection();

		// camera position movement
		const float moveSpeed = 10.0f * (Input::isKeyDown(Keys::LEFT_SHIFT) ? 2.0f : 1.0f);

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
	}

	const Mat view = Maths::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	Renderer::setViewMatrix(view);

	if (Input::isKeyJustReleased(Keys::V))
	{
		Window::endCursorCapture();
	}

	if (Input::isKeyJustReleased(Keys::C))
	{
		Window::beginCursorCapture();
	}

	ShapeRenderer::begin();

	//for (auto i = 0; i < gameObjects.size() / 2; ++i)
	//{
	//	auto& cube = gameObjects.at(i).first;
	//	const auto& colour = gameObjects.at(i).second;

	//	cube.setRotation(Window::currentTime() * 50, Window::currentTime() * 50, 0);
	//	ShapeRenderer::draw(cube, colour);
	//} 

	//for (auto i = gameObjects.size() / 2; i < gameObjects.size(); ++i)
	//{
	//	auto& cube = gameObjects.at(i).first;
	//	const auto& colour = gameObjects.at(i).second;

	//	ShapeRenderer::draw(cube, colour);
	//}

	if (Input::isMouseButtonDown(MouseButtons::LEFT))
	{
		auto dir = Camera::perspRayFromScreenPos(Input::getMousePos(), cameraPos);
		Line line(cameraPos, cameraPos + dir * 50.0f, 0.01f);

		gameObjects.emplace_back(line, Vec4{ 1.0f, 0.5f, 0.7f, 1.0f });
	}

	for (auto& [shape, colour] : gameObjects)
	{
		ShapeRenderer::draw(shape, colour);
	}

	drawAxes();

	ShapeRenderer::end();
}

void Application::imGuiRender()
{
	GUI::renderMenuBar();
	GUI::renderNewObjectMenu();
	GUI::renderToolbar();
}

void Application::destroy()
{
}