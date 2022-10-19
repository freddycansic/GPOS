#include "Application.h"

#include "engine/Colours.h"
#include "engine/GUI.h"

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

void drawAxes()
{
	using namespace Flags;

	static constexpr float AXES_LINE_WIDTH = 0.01f;
	static Line x(-100, 0, 0 , 100, 0, 0, AXES_LINE_WIDTH, Colours::RED);
	static Line z(0, 0, -100 , 0, 0, 100, AXES_LINE_WIDTH, Colours::BLUE);
	static Cube centreCube(0, 0, 0, 0.05f, Colours::WHITE);
	ObjectRenderer::draw(x, NO_DEPTH_TEST | NO_LIGHTING);
	ObjectRenderer::draw(z, NO_DEPTH_TEST | NO_LIGHTING);
	ObjectRenderer::draw(centreCube, NO_DEPTH_TEST | NO_LIGHTING);
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

	//Scene::handleMouseClicks();

	ObjectRenderer::begin();

	static constexpr float radius = 4.0f;
	static Vec3 lPos = { radius, 0, radius };

	lPos.x = radius * sin(Window::currentTime());
	lPos.y = radius * cos(Window::currentTime());

	ObjectRenderer::draw(Light(Camera::getPos(), {1, 1, 1}));
	ObjectRenderer::draw(Light(lPos, {1, 1, 1}));
	ObjectRenderer::draw(Cube(lPos, 0.3f, Colours::WHITE), Flags::NO_LIGHTING);

	ObjectRenderer::draw(Cube(-1.5f, 0, 0, 2.0f, Colours::WHITE));
	//ObjectRenderer::draw(Cube(1.5f, 0, 0, 2.0f, Vec3{0.1f, 0.1f, 0.1f}));

	//Scene::render();
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