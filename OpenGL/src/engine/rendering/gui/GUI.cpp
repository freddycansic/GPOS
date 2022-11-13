#include "GUI.h"

#include <array>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "engine/Window.h"
#include "engine/input/Input.h"
#include "engine/input/Keybind.h"
#include "engine/rendering/Renderer.h"
#include "engine/viewport/Scene.h"
#include "engine/rendering/object/Material.h"
#include "engine/rendering/object/shapes/Cube.h"

std::vector<std::pair<ImVec2, ImVec2>> s_PosAndSizeOfWindows;

void addCurrentWindowDimensions()
{
	s_PosAndSizeOfWindows.emplace_back
	(
		ImGui::GetWindowPos(),
		ImGui::GetWindowSize()
	);
}

namespace GUI
{
	void init()
	{
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(Window::GLFWWindow(), true);
		ImGui_ImplOpenGL3_Init();
		ImGui::StyleColorsDark();

		ImGui::GetIO().IniFilename = nullptr; // stops immgui from creating ini file
	}

	void startFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void endFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		s_PosAndSizeOfWindows.clear();
	}

	bool isMouseHoveringAnyWindows()
	{
		for (const auto& [pos, size] : s_PosAndSizeOfWindows)
		{
			if (isMouseHoveredWindow(ImGui::GetMousePos(), pos, size)) return true;
		}

		return false;
	}

	bool isMouseHoveredWindow(const ImVec2& mousePos, const ImVec2& windowPos, const ImVec2& windowSize)
	{
		return (mousePos.x > windowPos.x && mousePos.x < windowPos.x + windowSize.x && mousePos.y > windowPos.y && mousePos.y < windowPos.y + windowSize.y);
	}

	void renderMenuBar()
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("#1", reinterpret_cast<bool*>(1), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "Ctrl+O")) {}
				if (ImGui::MenuItem("Close", Input::getFunctionKeybind(Window::close).toString().c_str())) { Window::close(); }
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("Cube")) {}
				if (ImGui::MenuItem("Line")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Select"))
			{
				if (ImGui::MenuItem("Select All", "Ctrl+A")) {}
				if (ImGui::MenuItem("Deselect All", "Ctrl+D")) {}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				static const std::array projectionTypes = { "Perspective", "Orthographic" };
				static int selectedProjection = 0;
				ImGui::Combo("Projection", &selectedProjection, projectionTypes.data(), static_cast<int>(projectionTypes.size()));

				static const std::array drawingModes = { "Solid", "Wireframe" };
				static int selectedDrawingMode = 0;
				if (ImGui::Combo("Drawing Mode", &selectedDrawingMode, drawingModes.data(), static_cast<int>(drawingModes.size())))
				{
					switch (selectedDrawingMode)
					{
					case 0:
						Renderer::setRenderMode(RenderMode::Solid);
						break;

					case 1:
						Renderer::setRenderMode(RenderMode::Wireframe);
						break;

					default:
						break;
					}
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		addCurrentWindowDimensions();
		ImGui::End();
	}

	void renderProperties()
	{
		//ImGui::SetNextWindowPos({ static_cast<float>(Window::width() - 100), 50 });
	}

	void renderToolbar()
	{
		ImGui::SetNextWindowPos(ImVec2(0, 50));
		ImGui::SetNextWindowSize(ImVec2(0, 0));
		//ImGui::min
		ImGui::Begin("Toolbar", reinterpret_cast<bool*>(1), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

		ImGui::Text("%.1f FPS", static_cast<double>(ImGui::GetIO().Framerate));
		ImGui::Text("Time per frame %.4fms", static_cast<double>(ImGui::GetIO().DeltaTime) * 1000.0);

		//if (ImGui::Button("Translate"))
		//{
		//	Scene::setGizmoToTranslate();
		//}

		//if (ImGui::Button("Scale"))
		//{
		//	Scene::setGizmoToScale();
		//}

		//if (ImGui::Button("Rotate"))
		//{
		//	Scene::setGizmoToRotate();
		//}

		if (!Scene::getSelectedObjects().empty())
		{
			ImGui::ColorPicker4("Colour", &Scene::getSelectedObjects()[0]->material.colour.x);
		}

		addCurrentWindowDimensions();
		ImGui::End();
	}

	bool showingNewObjectMenu = false;
	ImVec2 mousePosOnShowWindow;

	void showNewObjectMenu()
	{
		showingNewObjectMenu = true;
		mousePosOnShowWindow = ImGui::GetMousePos();
	}

	void renderNewObjectMenu()
	{
		static const Material defaultMaterial = Vec4{ 0.2f, 0.2f, 0.2f, 1 };

		if (!showingNewObjectMenu) return;

		ImGui::SetNextWindowPos({ mousePosOnShowWindow.x - 1, mousePosOnShowWindow.y - 1 });
		ImGui::Begin("New", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

		if (ImGui::Button("Cube"))
		{
			Scene::addObject(std::make_unique<Cube>(0, 0, 0, 1.0f, defaultMaterial));
			showingNewObjectMenu = false;
		}

		//if (ImGui::Button("Line"))
		//{
		//	showingNewObjectMenu = false;
		//}

		const auto& windowPos = ImGui::GetWindowPos();
		const auto& windowSize = ImGui::GetWindowSize();
		const auto& realtimeMousePos = ImGui::GetMousePos();

		if (!isMouseHoveredWindow(realtimeMousePos, windowPos, windowSize))
		{
			showingNewObjectMenu = false;
		}

		addCurrentWindowDimensions();
		ImGui::End();
	}
}