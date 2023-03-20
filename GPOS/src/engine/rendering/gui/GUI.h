#pragma once

struct ImVec2;
struct WindowData;

namespace GUI
{
	void init();
	void startFrame();
	void endFrame();

	bool isMouseHoveringAnyWindows();

	void renderMenuBar();

	void renderToolbar();
	void renderProperties();
	void renderStats();
	void renderSceneViewer();

	void renderLauncher();

	void showNewObjectMenu();
	void renderNewObjectMenu();
}