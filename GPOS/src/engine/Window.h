#pragma once

#include <string>

struct GLFWwindow;

struct WindowConfig
{
	std::string title = "New Project";
	int width = 640;
	int height = 400;
	bool maximised = true;
	bool fullscreen = false; // TODO make fullscreen a functionality
	bool vsync = true;
	bool resizable = true;
	unsigned int samples = 4;
};

namespace Window
{
	void init(const WindowConfig& config);
	void destroy();

	void update();
	[[nodiscard]] int shouldClose();

	void close();
	bool closeCalled();

	void beginCursorCapture();
	void endCursorCapture();

	bool closeCalled();
	void close();
	
	// time
	[[nodiscard]] float currentTime();
	[[nodiscard]] float deltatime();

	void setHeight(int height);
	void setWidth(int width);
	[[nodiscard]] int width();
	[[nodiscard]] int height();
	[[nodiscard]] int displayHeight();
	[[nodiscard]] int displayWidth();
	[[nodiscard]] bool capturingCursor();

	void setTitle(const std::string& title);
	const std::string& getTitle();

	[[nodiscard]] GLFWwindow* GLFWWindow();
};

