#include "Application.h"

#include "engine/GUI.h"

#include "engine/input/Files.h"
#include "engine/input/Input.h"
#include "engine/input/Keys.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/Window.h"
#include "engine/rendering/objects/Line.h"
#include "engine/input/Keybind.h"
#include "engine/viewport/Camera.h"
#include "imgui/imgui.h"

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

unsigned int numLines = 0;

Cube c1(0, 0, 0, 1.0f);

//Cube getAABB(const Object& obj)
//{
//	float minX = 0, minY = 0, minZ = 0, maxX = 0, maxY = 0, maxZ = 0;
//	Vec3 avg;
//
//	for (const auto& pos : obj.getPositions())
//	{
//		minX = pos.x < minX ? pos.x : minX;
//		minY = pos.y < minY ? pos.y : minY;
//		minZ = pos.z < minZ ? pos.z : minZ;
//		maxX = pos.x > maxX ? pos.x : maxX;
//		maxY = pos.y > maxY ? pos.y : maxY;
//		maxZ = pos.z > maxZ ? pos.z : maxZ;
//
//		avg.x += pos.x;
//		avg.y += pos.y;
//		avg.z += pos.z;
//	}
//
//	const auto numPositions = static_cast<float>(obj.getPositions().size());
//
//	avg.x /= numPositions;
//	avg.y /= numPositions;
//	avg.z /= numPositions;
//
//	Cube AABB(avg, 1.0f);
//	AABB.setScale(maxX - minX, maxY - minY, maxZ - minZ);
//
//	return AABB;
//}


void Application::render()
{
	Renderer::clear(0.42f, 0.42f, 0.42f);

	if (Window::capturingCursor())
	{
		Camera::update();
	}
	
	if (Input::isKeyJustReleased(Keys::V))
	{
		Window::endCursorCapture();
	}

	if (Input::isKeyJustReleased(Keys::C))
	{
		Window::beginCursorCapture();
	}

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
		auto dir = Camera::perspRayFromCameraScreenPos(Input::getMousePos());
		Line line(Camera::getPos(), Camera::getPos() + dir * 50.0f, 0.01f);

		numLines++;

		gameObjects.emplace_back(line, Vec4{1.0f, 0.4f, 0.7f, 1.0f});
	}

	ShapeRenderer::begin();

	for (auto& [line, colour] : gameObjects)
	{
		ShapeRenderer::draw(line, colour);
	}

	c1.setRotation(Window::currentTime() * 50, Window::currentTime() * 50, 0);
	ShapeRenderer::draw(c1, { 1, 1, 0, 1 });

	//ShapeRenderer::draw(getAABB(c1), { 1.0f, 0, 0, 0.2f });
	ShapeRenderer::draw(c1.getAABB(), { 1.0f, 0, 0, 0.2f });
	
	drawAxes();

	ShapeRenderer::end();
}

void Application::imGuiRender()
{
	ImGui::Image(reinterpret_cast<ImTextureID>(tex1.getID()), ImVec2(100, 100));
	ImGui::Image(reinterpret_cast<ImTextureID>(tex2.getID()), ImVec2(100, 100));

	GUI::renderMenuBar();
	GUI::renderNewObjectMenu();
	GUI::renderToolbar();
}

void Application::destroy()
{
}