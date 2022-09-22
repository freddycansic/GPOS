#include "GUI.h"

#include <array>

#include "imgui/imgui.h"

void GUI::renderMenuBar()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("#1", reinterpret_cast<bool*>(1), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			if (ImGui::MenuItem("Close", "Ctrl+Q")) { abort(); }
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
			// TODO assign something to this to make it work
			static const std::array projectionTypes = { "Perspective", "Orthographic" };
			static int selectedProjection = 0;
			ImGui::Combo("Projection", &selectedProjection, projectionTypes.data(), projectionTypes.size());
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();
}
