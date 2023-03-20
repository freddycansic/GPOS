#include "Application.h"

#include <stack>

#include "imgui/imgui.h"

#include "engine/rendering/gui/GUI.h"
#include "engine/input/Files.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ObjectRenderer.h"
#include "engine/Window.h"
#include "engine/input/Buttons.h"
#include "engine/input/Keybind.h"
#include "engine/viewport/Camera.h"
#include "engine/viewport/Scene.h"
#include "engine/Util.h"
#include "engine/Debug.h"

void Application::init(const char* projectDir)
{
	std::cout << "Project " << (projectDir == nullptr ? "NO_PROJECT" : projectDir) << " loaded." << std::endl;

	if (projectDir) Scene::loadFromFile(projectDir);
	
	Window::beginCursorCapture();
}

void Application::render()
{
	Renderer::clear(Scene::getMutRefBackgroundColour());

	if ((Keybind({ MouseButtons::MOUSE_3 }).isHeld() || Keybind({ Keys::D }).isHeld()) && !GUI::isMouseHoveringAnyWindows())
	{
		if (!Window::capturingCursor()) Window::beginCursorCapture();

		Camera::update();
	} else
	{
		if (Window::capturingCursor()) Window::endCursorCapture();
	}
	
	Scene::handleMouseClicks();
	
	ObjectRenderer::begin();

		ObjectRenderer::draw(Light(Camera::getPos(), {1, 1, 1}));
		Scene::render();

	ObjectRenderer::end();
}

void Application::imGuiRender()
{
	GUI::renderLauncher();
	GUI::renderMenuBar();
	GUI::renderNewObjectMenu();
	GUI::renderToolbar();
	GUI::renderStats();
	GUI::renderSceneViewer();
	GUI::renderProperties();
}