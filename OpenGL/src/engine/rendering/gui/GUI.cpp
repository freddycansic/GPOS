#include "GUI.h"

#include <array>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "engine/Colours.h"
#include "engine/Stats.h"
#include "engine/Window.h"
#include "engine/input/Buttons.h"
#include "engine/input/Files.h"
#include "engine/input/Input.h"
#include "engine/input/Keybind.h"
#include "engine/rendering/Renderer.h"
#include "engine/viewport/Scene.h"
#include "engine/rendering/objects/Material.h"
#include "engine/rendering/objects/Cube.h"
#include "engine/rendering/gui/Gizmo.h"

enum class WindowType
{
	TOOLBAR,
	STATISTICS,
	PROPERTIES,
	MENU_BAR,
	NEW_OBJECT_MENU,
	SCENE_VIEWER
};

struct WindowData
{
	ImVec2 pos = {0, 0};
	ImVec2 size = {0, 0};
	bool visible = true;
};

constexpr float MENU_BAR_LENGTH = 17.0f; // px
constexpr float s_Padding = 20.0f;

std::unordered_map<WindowType, WindowData> s_Windows = 
{
	{WindowType::TOOLBAR, WindowData()},
	{WindowType::STATISTICS, WindowData()},
	{WindowType::PROPERTIES, WindowData()},
	{WindowType::MENU_BAR, WindowData()},
	{WindowType::NEW_OBJECT_MENU, {{0, 0}, {9999, 9999}, false}},
	{WindowType::SCENE_VIEWER, WindowData()}
};

void addCurrentWindowDimensions(const WindowType type)
{
	s_Windows[type].pos = ImGui::GetWindowPos();
	s_Windows[type].size = ImGui::GetWindowSize();
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
	}

	bool isMouseHoveringAnyWindows()
	{
		for (const auto& [windowType, window] : s_Windows)
		{
			if (isMouseHoveredWindow(ImGui::GetMousePos(), window)) return true;
		}

		return false;
	}

	bool isMouseHoveredWindow(const ImVec2& mousePos, const WindowData& data)
	{
		const ImVec2 windowPos = { data.pos.x - 5, data.pos.y - 5 };
		const ImVec2 windowSize = { data.size.x + 5, data.pos.y + 5 };
		return (data.visible && mousePos.x > windowPos.x && mousePos.x < windowPos.x + windowSize.x && mousePos.y > windowPos.y && mousePos.y < windowPos.y + windowSize.y);
	}

	void renderMenuBar()
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("#1", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "Ctrl+O")) {}

				if (ImGui::MenuItem("Load model", Input::getFunctionKeybind(Scene::loadModelIntoScene).toString().c_str()))
				{
					Scene::loadModelIntoScene();
				}

				if (ImGui::MenuItem("Close", Input::getFunctionKeybind(Window::close).toString().c_str()))
				{
					Window::close();
				}
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
				if (ImGui::MenuItem("Select All", Input::getFunctionKeybind(Scene::selectAll).toString().c_str()))
				{
					Scene::selectAll();
				}

				if (ImGui::MenuItem("Deselect All", Input::getFunctionKeybind(Scene::clearSelection).toString().c_str()))
				{
					Scene::clearSelection();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				ImGui::MenuItem("Toolbar", nullptr, &s_Windows.at(WindowType::TOOLBAR).visible);
				ImGui::MenuItem("Statistics", nullptr, &s_Windows.at(WindowType::STATISTICS).visible);
				ImGui::MenuItem("Scene Viewer", nullptr, &s_Windows.at(WindowType::SCENE_VIEWER).visible);
				ImGui::MenuItem("Properties", nullptr, &s_Windows.at(WindowType::PROPERTIES).visible);
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

				if (ImGui::Button("Change Background Colour"))
				{
					//ImGui::BeginPopup();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		addCurrentWindowDimensions(WindowType::MENU_BAR);
		ImGui::End();
	}

	void renderSceneViewer()
	{
		if (!s_Windows.at(WindowType::SCENE_VIEWER).visible) return;

		ImGui::SetNextWindowPos({ Window::width() - s_Padding - 200, s_Padding });
		ImGui::SetNextWindowSize({ 200, 400 });

		ImGui::Begin("Scene Viewer", nullptr);

		ImGui::PushItemWidth(-1);
		if (ImGui::BeginListBox("##"))
		{
			for (size_t i = 0; i < Scene::getObjects().size(); ++i)
			{
				const auto& object = Scene::getObjects().at(i);

				if (ImGui::Selectable((std::to_string(i) + std::string(" ") + object->stringName()).c_str(), object->selected))
				{
					if (!Keys::LEFT_CONTROL->isDown()) Scene::clearSelection();
					Scene::selectObject(&*object);
				}
			}

			ImGui::EndListBox();
		}

		addCurrentWindowDimensions(WindowType::SCENE_VIEWER);
		ImGui::End();
	}

	void renderProperties()
	{
		if (!s_Windows.at(WindowType::PROPERTIES).visible) return;

		if (Scene::getSelectedObjects().empty()) return;

		// snap to bottom of scene viewer
		const auto& sceneViewerDimensions = s_Windows.at(WindowType::SCENE_VIEWER);
		ImGui::SetNextWindowPos(ImVec2(sceneViewerDimensions.pos.x, sceneViewerDimensions.pos.y + sceneViewerDimensions.size.y));
		ImGui::SetNextWindowSize(ImVec2(sceneViewerDimensions.size.x, 0));

		ImGui::Begin("Properties", nullptr , ImGuiWindowFlags_NoScrollbar);
		
		const auto& lastSelected = Scene::getSelectedObjects().at(Scene::getSelectedObjects().size() - 1);
		const auto& lastSelectedTransform = lastSelected->getCombinedTransformations();

		ImGui::Text("Position : X:%.2f Y:%.2f Z:%.2f", lastSelectedTransform.tra.x, lastSelectedTransform.tra.y, lastSelectedTransform.tra.z);
		ImGui::Text("Rotation : X:%.2f Y:%.2f Z:%.2f", lastSelectedTransform.rot.x, lastSelectedTransform.rot.y, lastSelectedTransform.rot.z);
		ImGui::Text("Scale    : X:%.2f Y:%.2f Z:%.2f", lastSelectedTransform.sca.x, lastSelectedTransform.sca.y, lastSelectedTransform.sca.z);

		addCurrentWindowDimensions(WindowType::PROPERTIES);
		ImGui::End();
	}

	void renderToolbar()
	{
		if (!s_Windows.at(WindowType::TOOLBAR).visible) return;

		ImGui::SetNextWindowPos(ImVec2(s_Padding, s_Padding + MENU_BAR_LENGTH));
		ImGui::SetNextWindowSize(ImVec2(0, 0));
		//ImGui::min
		ImGui::Begin("Toolbar", reinterpret_cast<bool*>(1), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

		if (ImGui::Button("Translate"))
		{
			Gizmo::setTool(GizmoTool::TRANSLATE);
		}

		if (ImGui::Button("Scale"))
		{
			Gizmo::setTool(GizmoTool::SCALE);
		}

		if (ImGui::Button("Rotate"))
		{
			Gizmo::setTool(GizmoTool::ROTATE);
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
		if (!s_Windows.at(WindowType::STATISTICS).visible) return;

		ImGui::SetNextWindowPos(ImVec2(s_Padding, Window::height() - s_Windows.at(WindowType::STATISTICS).size.y - s_Padding));
		ImGui::Begin("Stats", nullptr);

		ImGui::Text("FPS : %.1f", static_cast<double>(ImGui::GetIO().Framerate));
		ImGui::Text("Time per frame : %.4fms", static_cast<double>(ImGui::GetIO().DeltaTime) * 1000.0);
		ImGui::Text("Draw calls per frame : %i", Stats::drawCallsPerFrame);

		addCurrentWindowDimensions(WindowType::STATISTICS);
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
			Scene::addObject<Cube>(0, 0, 0, 1.0f, Colours::DEFAULT);
			showingNewObjectMenu = false;
		}

		const auto& windowPos = ImGui::GetWindowPos();
		const auto& windowSize = ImGui::GetWindowSize();
		const auto& realtimeMousePos = ImGui::GetMousePos();

		if (!isMouseHoveredWindow(realtimeMousePos, WindowData(windowPos, windowSize, true)))
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