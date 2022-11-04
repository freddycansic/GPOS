#include "ApplicationLauncher.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "engine/Debug.h"
#include "engine/input/Input.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ObjectRenderer.h"

ApplicationLauncher::ApplicationLauncher(Application& app, const ApplicationConfig& config)
{
	Window::init(config.window);
	
	std::cout << glGetString(GL_VERSION) << std::endl;

	Debug::checkExtensionsSupported(
		"GL_ARB_bindless_texture",
		"GL_ARB_gpu_shader_int64"
	);

	Renderer::init();
	ObjectRenderer::init();

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(Window::GLFWWindow(), true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();

	app.init(config.projectDir);

	while (!Window::shouldClose() && !Window::closeCalled())
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Input::processFunctionKeybindPresses();

		app.render();
		app.imGuiRender();
	
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Input::resetMouseButtonStates()

		Window::update();
	}

	app.destroy();

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}
