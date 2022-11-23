#pragma once

struct ImVec2;

namespace GUI
{
	void init();
	void startFrame();
	void endFrame();

	bool isMouseHoveringAnyWindows();
	bool isMouseHoveredWindow(const ImVec2& mousePos, const ImVec2& windowPos, const ImVec2& windowSize);

	void renderMenuBar();

	void renderToolbar();
	void renderProperties();
	void renderStats();

	void showNewObjectMenu();
	void renderNewObjectMenu();

}
