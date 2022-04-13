#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

struct WindowConfig {
	std::string title = "Hello, world!";
	int width = 640;
	int height = 400;
	bool maxSize = false; // TODO
	bool fullscreen = false; // TODO
	bool vsync = true;
};

class Window
{
private:
	GLFWwindow* m_ID = nullptr;
	int m_Width = 0, m_Height = 0;
	float m_Delta = 0;

	static int s_DisplayWidth, s_DisplayHeight;
	static bool s_Initialised;

public:
	static void init();

	Window(const WindowConfig& config);
	~Window();
	Window(const Window& window) = delete;

	int shouldClose() const;
	void update();
	
	// time
	static float getCurrentTime();
	float getDelta() const { return m_Delta; }

	// getters
	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }
	GLFWwindow* getGLFWWindow() const { return m_ID; }
	
	static int getDisplayHeight();
	static int getDisplayWidth();
};

