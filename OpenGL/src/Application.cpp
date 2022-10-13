#include "Application.h"

#include "engine/Colours.h"
#include "engine/GUI.h"

#include "engine/input/Files.h"
#include "engine/input/Input.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/Window.h"
#include "engine/rendering/object/shapes/Line.h"
#include "engine/rendering/object/shapes/Cube.h"
#include "engine/rendering/object/Object.h"
#include "engine/input/Keybind.h"
#include "engine/rendering/object/Material.h"
#include "engine/viewport/Camera.h"
#include "engine/viewport/Scene.h"
#include "imgui/imgui.h"

void drawAxes()
{
	static constexpr float AXES_LINE_WIDTH = 0.01f;
	static Object x(std::make_unique<Line>(-100, 0, 0 , 100, 0, 0, AXES_LINE_WIDTH), Colours::RED);
	static Object z(std::make_unique<Line>(0, 0, -100 , 0, 0, 100, AXES_LINE_WIDTH), Colours::BLUE);
	static Object centreCube(std::make_unique<Cube>(0, 0, 0, 0.05f), Colours::WHITE);
	ShapeRenderer::draw(x);
	ShapeRenderer::draw(z);
	ShapeRenderer::draw(centreCube);
}

void Application::init(char* projectDir)
{
	openedProject = projectDir;
	std::cout << "Project " << (openedProject == nullptr ? "NO_PROJECT" : openedProject) << " loaded." << std::endl;

	tex1 = Texture(Files::internal("textures/image.png"));
	tex2 = Texture(Files::internal("textures/hashinshin.png"));

	Window::beginCursorCapture();
}

void Application::render()
{
	Renderer::clear(0.42f, 0.42f, 0.42f);

	if (Window::capturingCursor())
	{
		Camera::update();
	}

	Scene::handleMouseClicks();

	ShapeRenderer::begin();
	
	Scene::render();
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