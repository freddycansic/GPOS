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

	Renderer::init();

	//ImGui::CreateContext();
	//ImGui_ImplGlfw_InitForOpenGL(window.getGLFWWindow(), true);
	//ImGui_ImplOpenGL3_Init();
	//ImGui::StyleColorsDark();

	app.init();

	while (!window.shouldClose()) {
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();

		app.render();
		//app.imGuiRender();
	
		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.update();
	}

	app.destroy();

	//ImGui_ImplGlfw_Shutdown();
	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui::DestroyContext();
}
