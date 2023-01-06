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
#include "engine/viewport/Camera.h"

#define CHANGE_COLOUR_IF_SELECTED(selectedTool, currentTool, buttonCode)\
	if ((selectedTool) == (currentTool))\
	{\
		ImGui::PushStyleColor(ImGuiCol_Button, IMGUI_SELECTED_ORANGE); \
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IMGUI_SELECTED_HOVERING_ORANGE); \
	}\
	{buttonCode}\
	if ((selectedTool) == (currentTool))\
	{\
		ImGui::PopStyleColor(); \
		ImGui::PopStyleColor(); \
	}\

enum class WindowType
{
	TOOLBAR,
	STATISTICS,
	PROPERTIES,
	MENU_BAR,
	NEW_OBJECT_MENU,
	SCENE_VIEWER
};

constexpr float MENU_BAR_LENGTH = 17.0f; // px
constexpr float s_Padding = 20.0f;

std::unordered_map<WindowType, bool> s_WindowsVisible = 
{
	{WindowType::TOOLBAR, true},
	{WindowType::STATISTICS, true},
	{WindowType::PROPERTIES, true},
	{WindowType::MENU_BAR, true},
	{WindowType::NEW_OBJECT_MENU, false},
	{WindowType::SCENE_VIEWER, true}
};

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
		return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
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

			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("Select All", Input::getFunctionKeybind(Scene::selectAll).toString().c_str()))
				{
					Scene::selectAll();
				}

				if (ImGui::MenuItem("Deselect All", Input::getFunctionKeybind(Scene::clearSelection).toString().c_str()))
				{
					Scene::clearSelection();
				}

				if (ImGui::MenuItem("Delete Selected", Input::getFunctionKeybind(Scene::deleteSelected).toString().c_str()))
				{
					Scene::deleteSelected();
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				ImGui::MenuItem("Toolbar", nullptr, &s_WindowsVisible.at(WindowType::TOOLBAR));
				ImGui::MenuItem("Statistics", nullptr, &s_WindowsVisible.at(WindowType::STATISTICS));
				ImGui::MenuItem("Scene Viewer", nullptr, &s_WindowsVisible.at(WindowType::SCENE_VIEWER));
				ImGui::MenuItem("Properties", nullptr, &s_WindowsVisible.at(WindowType::PROPERTIES));
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

				static const std::array cameraModes = { "Orbital", "FPS-Style" };
				static int selectedCameraMode = 0;
				if (ImGui::Combo("Camera Mode", &selectedCameraMode, cameraModes.data(), static_cast<int>(cameraModes.size())))
				{
					switch (selectedCameraMode)
					{
					case 0:
						Camera::setMode(Camera::CameraMode::ORBIT);
						break;

					case 1:
						Camera::setMode(Camera::CameraMode::FPS_FLY);
						break;
					}
				}

				static constexpr auto CHANGE_SCENE_BACKGROUND_COLOUR = "CHANGE_SCENE_BACKGROUND_COLOUR";
				if (ImGui::Button("Change Background Colour"))
				{
					ImGui::OpenPopup(CHANGE_SCENE_BACKGROUND_COLOUR);
				}

				if (ImGui::BeginPopup(CHANGE_SCENE_BACKGROUND_COLOUR))
				{
					ImGui::ColorPicker3("##", &Scene::getMutRefBackgroundColour().x);
					ImGui::EndPopup();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
					
		ImGui::End();
	}

	void renderSceneViewer()
	{
		if (!s_WindowsVisible.at(WindowType::SCENE_VIEWER)) return;

		ImGui::SetNextWindowPos({ Window::width() - s_Padding - 200, s_Padding });
		ImGui::SetNextWindowSize({ 200, 0 });

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

		ImGui::End();
	}

	void renderProperties()
	{
		if (!s_WindowsVisible.at(WindowType::PROPERTIES)) return;

		if (Scene::getSelectedObjects().empty()) return;

		ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
		
		const auto& lastSelected = Scene::getSelectedObjects().at(Scene::getSelectedObjects().size() - 1);
		const auto& lastSelectedTransform = lastSelected->getCombinedTransformations();

		ImGui::Text("Position : X:%.2f Y:%.2f Z:%.2f", lastSelectedTransform.tra.x, lastSelectedTransform.tra.y, lastSelectedTransform.tra.z);
		ImGui::Text("Rotation : X:%.2f Y:%.2f Z:%.2f", lastSelectedTransform.rot.x, lastSelectedTransform.rot.y, lastSelectedTransform.rot.z);
		ImGui::Text("Scale    : X:%.2f Y:%.2f Z:%.2f", lastSelectedTransform.sca.x, lastSelectedTransform.sca.y, lastSelectedTransform.sca.z);

		ImGui::End();
	}

	void renderToolbar()
	{
		if (!s_WindowsVisible.at(WindowType::TOOLBAR)) return;

		//std::cout << "POS\n" <<  * reinterpret_cast<Vec2*>(&s_Windows.at(WindowType::TOOLBAR).pos) << std::endl;
		//std::cout << "SIZE\n" << *reinterpret_cast<Vec2*>(&s_Windows.at(WindowType::TOOLBAR).size) << std::endl;
		//auto cursor = ImGui::GetCursorPos();
		//std::cout << "MOUSE\n" << *reinterpret_cast<Vec2*>(&cursor) << std::endl;

		//std::cout << std::endl;

		ImGui::SetNextWindowPos(ImVec2(s_Padding, s_Padding + MENU_BAR_LENGTH));
		ImGui::Begin("Toolbar", reinterpret_cast<bool*>(1), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
		
		static constexpr auto IMGUI_SELECTED_HOVERING_ORANGE = ImVec4(Colours::SELECTION_ORANGE.x, Colours::SELECTION_ORANGE.y, Colours::SELECTION_ORANGE.z, 1.0f);

		static constexpr auto DARKENING = -0.1f;
		static constexpr auto IMGUI_SELECTED_ORANGE = ImVec4(Colours::SELECTION_ORANGE.x + DARKENING, Colours::SELECTION_ORANGE.y + DARKENING, Colours::SELECTION_ORANGE.z + DARKENING, 1.0f);

		const auto& tool = Gizmo::getTool();

		CHANGE_COLOUR_IF_SELECTED(tool, GizmoTool::TRANSLATE,
			if (ImGui::Button("Translate"))
			{
				Gizmo::setTool(GizmoTool::TRANSLATE);
			}
		);

		CHANGE_COLOUR_IF_SELECTED(tool, GizmoTool::SCALE,
			if (ImGui::Button("Scale"))
			{
				Gizmo::setTool(GizmoTool::SCALE);
			}
		);

		CHANGE_COLOUR_IF_SELECTED(tool, GizmoTool::ROTATE,
			if (ImGui::Button("Rotate"))
			{
				Gizmo::setTool(GizmoTool::ROTATE);
			}
		);

		if (!Scene::getSelectedObjects().empty())
		{
			ImGui::ColorPicker4("Colour", &Scene::getSelectedObjects()[0]->material.colour.x);
		}

		ImGui::End();
	}

	void renderStats()
	{
		if (!s_WindowsVisible.at(WindowType::STATISTICS)) return;

		ImGui::Begin("Stats", nullptr);

		ImGui::Text("FPS : %.1f", static_cast<double>(ImGui::GetIO().Framerate));
		ImGui::Text("Time per frame : %.4fms", static_cast<double>(ImGui::GetIO().DeltaTime) * 1000.0);
		ImGui::Text("Draw calls per frame : %i", Stats::drawCallsPerFrame);

		ImGui::End();
	}
	

	ImVec2 mousePosOnShowWindow;
	void showNewObjectMenu()
	{
		s_WindowsVisible.at(WindowType::NEW_OBJECT_MENU) = true;
		mousePosOnShowWindow = ImGui::GetMousePos();
	}

	void renderNewObjectMenu()
	{
		// TODO fix weird dragging thing
		if (!s_WindowsVisible.at(WindowType::NEW_OBJECT_MENU)) return;

		ImGui::SetNextWindowPos({ mousePosOnShowWindow.x - 1, mousePosOnShowWindow.y - 1 });
		ImGui::Begin("New", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

		if (ImGui::Button("Cube"))
		{
			Scene::addObject<Cube>(0, 0, 0, 1.0f, Colours::DEFAULT);
			s_WindowsVisible.at(WindowType::NEW_OBJECT_MENU) = false;
		}

		const auto& windowPos = ImGui::GetWindowPos();
		const auto& windowSize = ImGui::GetWindowSize();
		const auto& realtimeMousePos = ImGui::GetMousePos();

		if (!ImGui::IsWindowHovered() && !ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			s_WindowsVisible.at(WindowType::NEW_OBJECT_MENU) = false;
		}

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