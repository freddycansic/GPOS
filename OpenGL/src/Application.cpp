#include "Application.h"

#include "engine/Colours.h"
#include "engine/rendering/gui/GUI.h"

#include "engine/input/Files.h"
#include "engine/input/Input.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ObjectRenderer.h"
#include "engine/Window.h"
#include "engine/rendering/object/shapes/Line.h"
#include "engine/rendering/object/shapes/Cube.h"
#include "engine/input/Keybind.h"
#include "engine/viewport/Camera.h"
#include "engine/viewport/Scene.h"
#include "imgui/imgui.h"

using namespace Flags;

void drawAxes()
{
	static constexpr float AXES_LINE_WIDTH = 0.01f;
	static Line x(-100, 0, 0 , 100, 0, 0, AXES_LINE_WIDTH, Colours::RED);
	static Line z(0, 0, -100 , 0, 0, 100, AXES_LINE_WIDTH, Colours::BLUE);
	static Cube centreCube(0, 0, 0, 0.05f, Colours::WHITE);
	ObjectRenderer::draw(x, NO_LIGHTING);
	ObjectRenderer::draw(z, NO_LIGHTING);
	ObjectRenderer::draw(centreCube, NO_LIGHTING);
}

void Application::init(char* projectDir)
{
	openedProject = projectDir;
	std::cout << "Project " << (openedProject == nullptr ? "NO_PROJECT" : openedProject) << " loaded." << std::endl;

	tex1 = Texture(Files::internal("textures/image.png"));
	tex2 = Texture(Files::internal("textures/hashinshin.png"));

	//constexpr float cubesSideCount = 0;

	//for (float i = -cubesSideCount / 2; i < cubesSideCount / 2; ++i)
	//{
	//	for (float j = -cubesSideCount / 2; j < cubesSideCount / 2; ++j)
	//	{
	//		for (float k = -cubesSideCount / 2; k < cubesSideCount / 2; ++k)
	//		{
	//			Scene::addObject(std::make_unique<Cube>(i, j, k, 0.5f, Colours::WHITE));
	//		}
	//	}
	//}

	Window::beginCursorCapture();
}

void Application::render()
{
	//Renderer::clear(0.42f, 0.42f, 0.42f);
	Renderer::clear(Colours::BLACK);

	if (Window::capturingCursor())
	{
		Camera::update();
	}

	Scene::handleMouseClicks();

	ObjectRenderer::begin();

	static constexpr float radius = 5.0f;
	static Vec3 lPos = { radius, 2, radius };

	lPos.x = radius * sin(Window::currentTime());
	lPos.z = radius * cos(Window::currentTime());


	ObjectRenderer::draw(Light(lPos, {1, 1, 1}));
	Cube c(lPos, 0.5f, Colours::WHITE);
	ObjectRenderer::draw(c, NO_LIGHTING);

	Cube c0(1.5f, 0, 0, 2.0f, Colours::RED);
	ObjectRenderer::draw(c0);

	Cube c1(4.5f, 0, 0, 2.0f, Colours::GREEN);
	ObjectRenderer::draw(c1);

	ObjectRenderer::draw(Light(Camera::getPos(), {1, 1, 1}));
	Scene::render();
	drawAxes();

	ObjectRenderer::end();
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