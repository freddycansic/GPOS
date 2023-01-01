#pragma once

struct ImVec2;
struct WindowData;

namespace GUI
{
	void init();
	void startFrame();
	void endFrame();

	bool isMouseHoveringAnyWindows();
	bool isMouseHoveredWindow(const ImVec2& mousePos, const WindowData& data);

	void renderMenuBar();

	void renderToolbar();
	void renderProperties();
	void renderStats();
	void renderSceneViewer();

	void showNewObjectMenu();
	void renderNewObjectMenu();

	void showLeo();
}
