#include "GUI.h"

#include <array>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "engine/Colours.h"
#include "engine/Stats.h"

#include "engine/Window.h"
#include "engine/input/Files.h"
#include "engine/input/Input.h"
#include "engine/input/Keybind.h"
#include "engine/rendering/Renderer.h"
#include "engine/viewport/Scene.h"
#include "engine/rendering/objects/Material.h"
#include "engine/rendering/objects/Cube.h"

enum class WindowType
{
	TOOLBAR,
	STATISTICS,
	PROPERTIES,
	MENU_BAR,
	NEW_OBJECT_MENU
};

static constexpr float MENU_BAR_LENGTH = 17.0f; // px
static const ImVec2 s_Padding = { 20.0f, 20.0f + MENU_BAR_LENGTH };
static std::unordered_map<WindowType, std::pair<ImVec2, ImVec2>> s_PosAndSizeOfWindows;

void addCurrentWindowDimensions(const WindowType type)
{
	s_PosAndSizeOfWindows[type] = 
	{
		ImGui::GetWindowPos(),
		ImGui::GetWindowSize()
	};
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
		for (const auto& [windowType, posAndSize] : s_PosAndSizeOfWindows)
		{
			const auto& pos = posAndSize.first;
			const auto& size = posAndSize.second;
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

				if (ImGui::MenuItem("Load model"))
				{
					if (const auto path = Files::getPathFromDialogue("obj"); path)
					{
						Scene::loadModel(path);
					}
				}

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
					}
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		addCurrentWindowDimensions(WindowType::MENU_BAR);
		ImGui::End();
	}

	void renderProperties()
	{
		static ImVec2 ls_WindowSize;

		if (Scene::getSelectedObjects().empty()) return;

		ImGui::SetNextWindowPos({ static_cast<float>(Window::width()) - ls_WindowSize.x - s_Padding.x, s_Padding.y });
		ImGui::SetNextWindowSize({ 0, 0 });

		ImGui::Begin("Properties", nullptr , ImGuiWindowFlags_NoScrollbar);
		
		const auto& lastSelected = Scene::getSelectedObjects().at(Scene::getSelectedObjects().size() - 1);
		const auto& lastSelectedTransform = lastSelected->getCombinedTransformations();

		ImGui::Text("Position : X:%.2f Y:%.2f Z:%.2f", lastSelectedTransform.tra.x, lastSelectedTransform.tra.y, lastSelectedTransform.tra.z);
		ImGui::Text("Rotation : X:%.2f Y:%.2f Z:%.2f", lastSelectedTransform.rot.x, lastSelectedTransform.rot.y, lastSelectedTransform.rot.z);
		ImGui::Text("Scale    : X:%.2f Y:%.2f Z:%.2f", lastSelectedTransform.sca.x, lastSelectedTransform.sca.y, lastSelectedTransform.sca.z);

		ls_WindowSize = ImGui::GetWindowSize();

		addCurrentWindowDimensions(WindowType::PROPERTIES);
		ImGui::End();
	}

	void renderToolbar()
	{
		ImGui::SetNextWindowPos(s_Padding);
		ImGui::SetNextWindowSize(ImVec2(0, 0));
		//ImGui::min
		ImGui::Begin("Toolbar", reinterpret_cast<bool*>(1), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

		if (ImGui::Button("Translate"))
		{
			Scene::setGizmoToTranslate();
		}

		if (ImGui::Button("Scale"))
		{
			Scene::setGizmoToScale();
		}

		if (ImGui::Button("Rotate"))
		{
			Scene::setGizmoToRotate();
		}

		if (!Scene::getSelectedObjects().empty())
		{
			ImGui::ColorPicker4("Colour", &Scene::getSelectedObjects()[0]->material.colour.x);
		}

		addCurrentWindowDimensions(WindowType::TOOLBAR);
		ImGui::End();
	}

	void renderStats()
	{
		ImGui::Begin("Stats", nullptr);

		ImGui::Text("FPS : %.1f", static_cast<double>(ImGui::GetIO().Framerate));
		ImGui::Text("Time per frame : %.4fms", static_cast<double>(ImGui::GetIO().DeltaTime) * 1000.0);
		ImGui::Text("Draw calls per frame : %i", Stats::drawCallsPerFrame);

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
		if (!showingNewObjectMenu) return;

		ImGui::SetNextWindowPos({ mousePosOnShowWindow.x - 1, mousePosOnShowWindow.y - 1 });
		ImGui::Begin("New", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

		if (ImGui::Button("Cube"))
		{
			Scene::addObject(std::make_unique<Cube>(0, 0, 0, 1.0f, Colours::DEFAULT));
			showingNewObjectMenu = false;
		}

		const auto& windowPos = ImGui::GetWindowPos();
		const auto& windowSize = ImGui::GetWindowSize();
		const auto& realtimeMousePos = ImGui::GetMousePos();

		if (!isMouseHoveredWindow(realtimeMousePos, windowPos, windowSize))
		{
			showingNewObjectMenu = false;
		}

		addCurrentWindowDimensions(WindowType::NEW_OBJECT_MENU);
		ImGui::End();
	}

	void showLeo()
	{
		static const Texture leo(Files::internal("textures/leo.png"));

		ImGui::SetNextWindowPos(ImGui::GetMousePos());
		ImGui::Begin("LEO", nullptr);
		ImGui::Image(reinterpret_cast<ImTextureID>(leo.getID()), ImVec2(200, 200));
		ImGui::End();
	}
}