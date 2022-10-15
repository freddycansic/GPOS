#include "GUI.h"

#include <array>

#include "Util.h"
#include "Window.h"
#include "imgui/imgui.h"
#include "input/Input.h"
#include "engine/input/Keybind.h"
#include "rendering/Renderer.h"
#include "engine/viewport/Scene.h"
#include "rendering/object/Material.h"
#include "rendering/object/shapes/Cube.h"

void GUI::renderMenuBar()
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
	ImGui::End();
}

void GUI::renderToolbar()
{
	ImGui::SetNextWindowPos(ImVec2(0, 50));
	ImGui::Begin("Toolbar", reinterpret_cast<bool*>(1), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

	ImGui::Text("%.1f FPS", static_cast<double>(ImGui::GetIO().Framerate));

	ImGui::End();
}

bool showingNewObjectMenu = false;
ImVec2 mousePosOnShowWindow;

void GUI::showNewObjectMenu()
{
	showingNewObjectMenu = true;
	mousePosOnShowWindow = ImGui::GetMousePos();
}

void GUI::renderNewObjectMenu()
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

	if (ImGui::Button("Line"))
	{
		showingNewObjectMenu = false;
	}

	const auto& windowPos = ImGui::GetWindowPos();
	const auto& windowSize = ImGui::GetWindowSize();
	const auto& realtimeMousePos = ImGui::GetMousePos();

	if (!Util::isMouseHoveredWindow(realtimeMousePos, windowPos, windowSize))
	{
		showingNewObjectMenu = false;
	}

	ImGui::End();
}