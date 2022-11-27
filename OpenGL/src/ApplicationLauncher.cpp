#include "ApplicationLauncher.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "engine/Debug.h"
#include "engine/Stats.h"
#include "engine/input/Input.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ObjectRenderer.h"
#include "engine/rendering/gui/GUI.h"
#include "engine/viewport/Camera.h"

ApplicationLauncher::ApplicationLauncher(Application& app, const ApplicationConfig& config)
{
	Window::init(config.window);

#ifdef DEBUG
	std::cout << "Running in DEBUG mode." << std::endl;
#else
	std::cout << "Running in RELEASE mode." << std::endl;
#endif

	std::cout << glGetString(GL_VERSION) << std::endl;

	Debug::checkExtensionsSupported(
		"GL_ARB_bindless_texture"
	);

	Renderer::init();
	ObjectRenderer::init();
	GUI::init();

	app.init(config.projectDir);

	Camera::update();

	while (!Window::shouldClose() && !Window::closeCalled())
	{
		GUI::startFrame();

		Input::processFunctionKeybindPresses();

		app.imGuiRender();
		Stats::resetPerFrameData();
		app.render();
	
		GUI::endFrame();

		Window::update();

	}

	app.destroy();

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}
