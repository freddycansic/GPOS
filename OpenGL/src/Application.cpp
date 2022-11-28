#include "Application.h"

#include "imgui/imgui.h"

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

using namespace Flags;

void drawAxes()
{
	static constexpr float AXES_LINE_WIDTH = 0.01f;
	static Line x(-100, 0, 0 , 100, 0, 0, AXES_LINE_WIDTH, Colours::RED);
	static Line z(0, 0, -100 , 0, 0, 100, AXES_LINE_WIDTH, Colours::BLUE);
	static Cube centreCube(0, 0, 0, 0.05f, Colours::WHITE);
	ObjectRenderer::draw(centreCube, NO_LIGHTING);
	ObjectRenderer::draw(x, NO_LIGHTING);
	ObjectRenderer::draw(z, NO_LIGHTING);
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
	Renderer::clear(Colours::BLACK);

	if (Input::isMouseButtonDown(MouseButtons::MIDDLE) || Input::isKeyDown(Keys::D))
	{
		if (!Window::capturingCursor()) Window::beginCursorCapture();

		Camera::update();
	} else
	{
		if (Window::capturingCursor()) Window::endCursorCapture();
	}

	if (!GUI::isMouseHoveringAnyWindows())
	{
		Scene::handleMouseClicks(); // only process mouse clicks when we're not interacting with the GUI
	}

	ObjectRenderer::begin();

	ObjectRenderer::draw(Light(Camera::getPos(), {1, 1, 1}));
	Scene::render();
	drawAxes();

	ObjectRenderer::end();

}

void Application::imGuiRender()
{
	//ImGui::ShowDemoWindow();

	GUI::renderMenuBar();
	GUI::renderNewObjectMenu();
	GUI::renderToolbar();
	GUI::renderStats();
	GUI::renderProperties();

	ImGui::SetNextWindowPos(ImVec2(static_cast<float>(Window::width()) - 400.0f, 150.0f));
	ImGui::Begin("Instructions", reinterpret_cast<bool*>(1));
	ImGui::Text("Hold middle mouse = orbit camera\nScroll = zoom in / out\nShift + A = new object menu\nLeft click = select object\nLeft ctrl + left click = select multiple objects\nF = set last selected object as orbit point\nG = select move tool\nS = select scale tool\nR = select rotate tool");
	ImGui::End();
}

void Application::destroy()
{
}