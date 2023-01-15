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
#include "engine/rendering/objects/Model.h"
#include "engine/rendering/gui/Gizmo.h"
#include "engine/viewport/Camera.h"
#include "imgui/imgui_internal.h"

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
	SCENE_VIEWER,
	LAUNCHER
};

constexpr float MENU_BAR_LENGTH = 17.0f; // px
constexpr float PADDING = 15.0f;
constexpr float TOOLTIP_HOVER_TIMER_THRESHOLD = 0.4f;
constexpr float DEFAULT_CUBE_SIZE = 1.4f;

std::unordered_map<WindowType, bool> s_WindowsVisible = 
{
	{WindowType::TOOLBAR, true},
	{WindowType::STATISTICS, true},
	{WindowType::PROPERTIES, true},
	{WindowType::MENU_BAR, true},
	{WindowType::NEW_OBJECT_MENU, false},
	{WindowType::SCENE_VIEWER, true},
	{WindowType::LAUNCHER, true}
};

void addRect(ImVec2 size, const ImVec4& col)
{
	auto dl = ImGui::GetWindowDrawList();

	const auto windowPos = ImGui::GetWindowPos();
	const auto x = windowPos.x + ImGui::GetCursorPosX();
	const auto y = windowPos.y + ImGui::GetCursorPosY();

	dl->AddRect(ImVec2(x, y), ImVec2(x + size.x, y + size.y), IM_COL32(col.x * 255, col.y * 255, col.z * 255, col.w * 255));
	ImGui::Dummy(ImVec2(size.x, size.y));
}

void addFilledRect(ImVec2 size, const ImVec4& col)
{
	auto dl = ImGui::GetWindowDrawList();

	const auto windowPos = ImGui::GetWindowPos();
	const auto x = windowPos.x + ImGui::GetCursorPosX();
	const auto y = windowPos.y + ImGui::GetCursorPosY();

	dl->AddRectFilled(ImVec2(x, y), ImVec2(x + size.x, y + size.y), IM_COL32(col.x * 255, col.y * 255, col.z * 255, col.w * 255));
	ImGui::Dummy(ImVec2(size.x, size.y));
}

bool isItemHovered(float timer = 0, ImGuiHoveredFlags flags = 0)
{
	return ImGui::IsItemHovered(flags) && GImGui->HoveredIdTimer > timer;
}

namespace GUI
{
	void init()
	{
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(Window::GLFWWindow(), true);
		ImGui_ImplOpenGL3_Init();
		ImGui::StyleColorsDark();

		auto& io = ImGui::GetIO();
		constexpr float SIZE_PIXELS = 13;

		io.Fonts->AddFontFromFileTTF("res/fonts/Cousine-Regular.ttf", SIZE_PIXELS);
		io.IniFilename = nullptr; // stops immgui from creating ini file
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
		// if mouse is hovering window and clicks return true until the mouse is no longer clicking
		static bool clickingInWindow = false;

		if (clickingInWindow && ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			return true;
		}

		clickingInWindow = false;

		const auto& window = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_RootAndChildWindows);
		const auto& item = ImGui::IsAnyItemHovered(); // TODO fix mouse hovering colour picker window not working
		const auto& any = window || item;

		if (any && ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			clickingInWindow = true;
			return true;
		}

		return any;
	}
	
	void renderMenuBar()
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::Begin("#1", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", Input::getFunctionKeybind(Scene::loadFromFile).toString().c_str())) { Scene::loadFromFile(); }

				if (ImGui::MenuItem("Save", Input::getFunctionKeybind(Scene::saveToFile).toString().c_str())) { Scene::saveToFile(); }

				if (ImGui::MenuItem("Load model", Input::getFunctionKeybind(Scene::loadModelIntoScene).toString().c_str())) { Scene::loadModelIntoScene(); }

				if (ImGui::MenuItem("Close", Input::getFunctionKeybind(Window::close).toString().c_str())) { Window::close(); }

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("Cube")) { Scene::addModel(Model("res/models/cube.obj", 0, 0, 0, 0, 1.0f, Colours::DEFAULT)); }

				if (ImGui::MenuItem("Plane")) { Scene::addModel(Model("res/models/plane.obj", 0, 0, 0, 0, 1.0f, Colours::DEFAULT)); }

				if (ImGui::MenuItem("Sphere")) { Scene::addModel(Model("res/models/sphere.obj", 0, 0, 0, 0, 1.0f, Colours::DEFAULT)); }

				if (ImGui::MenuItem("Cylinder")) { Scene::addModel(Model("res/models/cylinder.obj", 0, 0, 0, 0, 1.0f, Colours::DEFAULT)); }

				if (ImGui::MenuItem("Cone")) { Scene::addModel(Model("res/models/cone.obj", 0, 0, 0, 0, 1.0f, Colours::DEFAULT)); }

				if (ImGui::MenuItem("Torus")) { Scene::addModel(Model("res/models/torus.obj", 0, 0, 0, 0, 1.0f, Colours::DEFAULT)); }
				
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("Select All", Input::getFunctionKeybind(Scene::selectAll).toString().c_str())) { Scene::selectAll(); }

				if (ImGui::MenuItem("Deselect All", Input::getFunctionKeybind(Scene::clearSelection).toString().c_str())) { Scene::clearSelection(); }

				if (ImGui::MenuItem("Delete Selected", Input::getFunctionKeybind(Scene::deleteSelected).toString().c_str()))
				{
					Scene::deleteSelected();
				}

				if (ImGui::MenuItem("Invert Selection", Input::getFunctionKeybind(Scene::invertSelection).toString().c_str()))
				{
					Scene::invertSelection();
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
				static const std::array drawingModes = { "Solid", "Wireframe" };
				static int selectedDrawingMode = 0;
				if (ImGui::Combo("Drawing Mode", &selectedDrawingMode, drawingModes.data(), static_cast<int>(drawingModes.size())))
				{
					switch (selectedDrawingMode)
					{
						case 0:
							Renderer::setRenderMode(RenderMode::SOLID);
							break;

						case 1:
							Renderer::setRenderMode(RenderMode::WIREFRAME);
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
		auto& visible = s_WindowsVisible.at(WindowType::SCENE_VIEWER);
		if (!visible) return;

		ImGui::SetNextWindowPos({ Window::width() - PADDING - 200, PADDING });
		ImGui::SetNextWindowSize({ 200, 0 });

		ImGui::Begin("Scene Viewer", &visible);

		ImGui::PushItemWidth(-1);
		if (ImGui::BeginListBox("##"))
		{
			for (size_t i = 0; i < Scene::getModels().size(); ++i)
			{
				auto& model = Scene::getModels().at(i);

				if (ImGui::Selectable((std::to_string(i) + std::string("| ") + model->stringName()).c_str(), model->selected, ImGuiSelectableFlags_AllowDoubleClick))
				{
					if (!Keys::LEFT_CONTROL->isDown()) Scene::clearSelection();

					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						Camera::setOrbitTarget(model->getAvgPosition());
						Camera::update();
					}

					Scene::selectModel(model);
				}
			}

			ImGui::EndListBox();
		}

		ImGui::End();
	}

	void renderLauncher()
	{
		auto& visible = s_WindowsVisible.at(WindowType::LAUNCHER);
		if (!visible) return;

		static constexpr ImVec2 LAUNCHER_SIZE = {600, 400};

		ImGui::SetNextWindowPos(ImVec2(Window::width() / 2 - LAUNCHER_SIZE.x / 2, Window::height() / 2 - LAUNCHER_SIZE.y / 2));
		ImGui::Begin("Launcher", &visible, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

		ImGui::Image(reinterpret_cast<ImTextureID>(Scene::getTexture("res/textures/GPOS_title.png").getID()), ImVec2(590, 160));

		// title
		const auto gposText = "Graphical Positioning Orientation Scaling";
		auto textWidth = ImGui::CalcTextSize(gposText).x;
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - textWidth / 2);
		ImGui::Text(gposText);

		ImGui::Separator();

		// list of old projects
		if (ImGui::BeginListBox("##", ImVec2(LAUNCHER_SIZE.x, 0)))
		{
			for (const auto& [path, time] : Scene::getPreviousScenes())
			{
				// center selectables
				textWidth = ImGui::CalcTextSize(path.c_str()).x;
				ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - textWidth / 2);

				if (ImGui::Selectable(path.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(textWidth, 0)))
				{
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						Scene::loadFromFile(path.c_str());
						visible = false;
						break;
					}
				}
			}

			ImGui::EndListBox();
		}

		ImGui::Separator();

		// new project button
		const auto newProjectText = "New Project";
		textWidth = ImGui::CalcTextSize(newProjectText).x;
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - textWidth / 2 - 10);

		if (ImGui::Button(newProjectText))
		{
			visible = false;
		}
		
		ImGui::End();
	}

	void renderProperties()
	{
		if (Scene::getSelectedModels().empty()) return;

		auto& visible = s_WindowsVisible.at(WindowType::PROPERTIES);
		if (!visible) return;

		static float windowHeight = 0.0f;

		// gross workaround but idk
		if (static size_t numFramesVisible = 0; numFramesVisible <= 2)
		{
			// move out of frame until window height is calculated
			ImGui::SetNextWindowPos(ImVec2(-999, 0));
			numFramesVisible++;
		}
		else
		{
			ImGui::SetNextWindowPos(ImVec2(PADDING, static_cast<float>(Window::height()) / 2 - windowHeight / 2));
		}

		ImGui::Begin("Properties", &visible, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

		const auto& selectedModels = Scene::getSelectedModels();
		const auto& lastSelected = selectedModels.at(Scene::getSelectedModels().size() - 1);
		const auto& [tra, rot, sca] = lastSelected->getCombinedTransformations();

		ImGui::Text("Position : X:%.2f Y:%.2f Z:%.2f", tra.x, tra.y, tra.z);
		ImGui::Text("Rotation : X:%.2f Y:%.2f Z:%.2f", rot.x, rot.y, rot.z);
		ImGui::Text("Scale    : X:%.2f Y:%.2f Z:%.2f", sca.x, sca.y, sca.z);

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::ColorPicker4("##", &lastSelected->material.colour.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview);

		// custom colour preview
		ImGui::Spacing();
		addFilledRect(ImVec2(ImGui::GetItemRectSize().x - ImGui::GetStyle().FramePadding.x / 2, 20), *reinterpret_cast<ImVec4*>(&lastSelected->material.colour));

		ImGui::Spacing();
		ImGui::Separator();
		
		if (ImGui::Button("Open Image..."))
		{
			const auto& texturePath = Files::getPathFromDialogue();
			if (texturePath) lastSelected->material.texturePath = texturePath;
		}

		constexpr float IMAGE_SIZE = 200;
		if (!lastSelected->material.texturePath.empty())
		{
			ImGui::Image(reinterpret_cast<ImTextureID>(Scene::getTexture(lastSelected->material.texturePath).getID()), ImVec2(IMAGE_SIZE, IMAGE_SIZE));
		} else
		{
			ImGui::SameLine();
			ImGui::Text("No Texture");
			addRect(ImVec2(IMAGE_SIZE, IMAGE_SIZE), ImGui::GetStyle().Colors[ImGuiCol_Button]);
		}

		windowHeight = ImGui::GetWindowHeight();
		ImGui::End();
	}

	void renderToolbar()
	{
		auto& visible = s_WindowsVisible.at(WindowType::TOOLBAR);
		if (!visible) return;

		ImGui::SetNextWindowPos(ImVec2(PADDING, PADDING + MENU_BAR_LENGTH));
		ImGui::Begin("Toolbar", &visible, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
		
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

		ImGui::End();
	}

	void renderStats()
	{
		auto& visible = s_WindowsVisible.at(WindowType::STATISTICS);
		if (!visible) return;

		static ImVec2 statsSize;
		ImGui::SetNextWindowPos(ImVec2(PADDING, Window::height() - PADDING - statsSize.y));

		ImGui::Begin("Stats", &visible, ImGuiWindowFlags_AlwaysAutoResize);

		ImGui::Text("FPS : %.1f", static_cast<double>(ImGui::GetIO().Framerate));
		ImGui::Text("Time per frame : %.4fms", static_cast<double>(ImGui::GetIO().DeltaTime) * 1000.0);
		ImGui::Text("Draw calls per frame : %zu", Stats::drawCallsPerFrame);
		ImGui::Text("Num vertices per frame : %zu", Stats::numVerticesPerFrame);

		statsSize = ImGui::GetWindowSize();

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
		auto& visible = s_WindowsVisible.at(WindowType::NEW_OBJECT_MENU);
		if (!visible) return;

		static constexpr float LEEWAY = 3;
		ImGui::SetNextWindowPos({ mousePosOnShowWindow.x - LEEWAY, mousePosOnShowWindow.y - LEEWAY });
		ImGui::Begin("New", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);

		if (ImGui::Button("Cube"))
		{
			Scene::addModel(Model("res/models/cube.obj", 0, 0, 0, 0, 1.0f, Colours::DEFAULT));
			visible = false;
		}

		if (ImGui::Button("Plane"))
		{
			Scene::addModel(Model("res/models/plane.obj", 0, 0, 0, 0, 1.0f, Colours::DEFAULT));
			visible = false;
		}

		if (ImGui::Button("Sphere"))
		{
			Scene::addModel(Model("res/models/sphere.obj", 0, 0, 0, 0, 1.0f, Colours::DEFAULT));
			visible = false;
		}

		if (ImGui::Button("Cylinder"))
		{
			Scene::addModel(Model("res/models/cylinder.obj", 0, 0, 0, 0, 1.0f, Colours::DEFAULT));
			visible = false;
		}

		if (ImGui::Button("Cone"))
		{
			Scene::addModel(Model("res/models/cone.obj", 0, 0, 0, 0, 1.0f, Colours::DEFAULT));
			visible = false;
		}

		if (ImGui::Button("Torus"))
		{
			Scene::addModel(Model("res/models/torus.obj", 0, 0, 0, 0, 1.0f, Colours::DEFAULT));
			visible = false;
		}

		const auto [wx, wy] = ImGui::GetWindowPos();
		const auto [w, h] = ImGui::GetWindowSize();
		const auto [cx, cy] = Input::getMousePos(); // idk why imgui cursor pos doesnt update itself

		if (!(cx > wx && cy > wy && cx < wx + w && cy < wy + h))
		{
			s_WindowsVisible.at(WindowType::NEW_OBJECT_MENU) = false;
		}

		ImGui::End();
	}
}