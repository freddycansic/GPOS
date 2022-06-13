#pragma once

#include <GLFW/glfw3.h>
#include <string>

struct WindowConfig
{
	std::string title = "Hello, world!";
	int width = 640;
	int height = 400;
	bool maxSize = false;
	bool fullscreen = false; // TODO make fullscreen a functionality
	bool vsync = true;
};

namespace Window
{

	void init(const WindowConfig& config);
	void destroy();

	void update();
	[[nodiscard]] int shouldClose();
	
	// time
	[[nodiscard]] float currentTime();
	[[nodiscard]] float deltatime();

	// getters
	[[nodiscard]] int width();
	[[nodiscard]] int height();
	[[nodiscard]] int displayHeight();
	[[nodiscard]] int displayWidth();

	[[nodiscard]] GLFWwindow* GLFWWindow();
};

