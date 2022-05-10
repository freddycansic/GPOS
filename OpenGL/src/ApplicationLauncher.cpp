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

	int numExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	std::vector<const char*> usedExtensions = {"GL_ARB_bindless_texture"};

	for (unsigned int i = 0; i < numExtensions; i++) {
		const char* supportedExtension = (const char*)glGetStringi(GL_EXTENSIONS, i);
		
		for (const auto& extension : usedExtensions) {
			if (strcmp(supportedExtension, extension) == 0) {
				std::cout << "SUPPORTED : " << extension << std::endl;
			}
		}
	}

	Renderer::init(window);

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window.getGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();

	app.windowWidth = window.getDisplayWidth();
	app.windowHeight = window.getDisplayHeight();
	app.init();

	unsigned int frame = 1;
	while (!window.shouldClose()) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		app.render();
		app.imGuiRender();
	
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.update();
		std::cout << frame++ << std::endl;
	}

	app.destroy();

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}
