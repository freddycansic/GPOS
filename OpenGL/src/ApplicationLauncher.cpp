#include "ApplicationLauncher.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "engine/Debug.h"
#include "engine/Input.h"
#include "engine/Keys.h"
#include "engine/rendering/Renderer.h"

ApplicationLauncher::ApplicationLauncher(Application& app, const ApplicationConfig& config, char* projectDir)
{
	Window::init(config.window);
	
	std::cout << glGetString(GL_VERSION) << std::endl;

	Debug::checkExtensionsSupported(
		"GL_ARB_bindless_texture",
		"GL_ARB_gpu_shader_int64"
	);

	Renderer::init();

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(Window::GLFWWindow(), true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();

	app.init(projectDir);

	while (!Window::shouldClose()) {
		if (Input::isKeyDown(Keys::ESCAPE)) break;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		app.render();
		app.imGuiRender();
	
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Window::update();
	}

	app.destroy();

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}
