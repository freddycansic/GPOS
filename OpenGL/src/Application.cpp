#include "Application.h"

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
#include "maths/Vectors.h"

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

	Material coloured = Vec4{ 1, 0, 0, 1 };
	Material colouredTexture(tex1, {1, 0, 0, 1});

	Scene::addObject(Object(std::make_unique<Cube>(0, 0, 0, 1), coloured));
	Scene::addObject(Object(std::make_unique<Cube>(2, 0, 0, 1), colouredTexture));

	Window::beginCursorCapture();
}

void Application::render()
{
	Renderer::clear(0.42f, 0.42f, 0.42f);

	if (Window::capturingCursor())
	{
		Camera::update();
	}

	if (Input::isMouseButtonDown(MouseButtons::LEFT))
	{
		for ()
	}

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