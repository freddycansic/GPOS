#include "ApplicationLauncher.h"

#include "engine/rendering/Renderer.h"
#include "engine/rendering/ShapeRenderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

ApplicationLauncher::ApplicationLauncher(Application& app, const ApplicationConfig& config)
{
	Window::init();
	Window window(config.window);
	
	std::cout << glGetString(GL_VERSION) << std::endl;

	const std::vector<const char*> usedExtensions = {
		"GL_ARB_bindless_texture",
	};

	for (const auto& extension : usedExtensions) {
		std::cout << extension << " : " << (glfwExtensionSupported(extension) ? "" : "NOT ") << "SUPPORTED" << std::endl;
	}

	Renderer::init(window);

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window.getGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();

	app.windowWidth = Window::getDisplayWidth();
	app.windowHeight = Window::getDisplayHeight();
	app.init();

	while (!window.shouldClose()) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		app.render();
		app.imGuiRender();
	
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.update();
	}

	app.destroy();

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}
