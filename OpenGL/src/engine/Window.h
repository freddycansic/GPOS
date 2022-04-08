#pragma once

#include <GLFW/glfw3.h>
#include <string>

class Window
{
private:
	GLFWwindow* m_ID;
	int m_Width, m_Height, m_DisplayWidth, m_DisplayHeight;
	float m_Delta = 0;
public:

	Window(int width, int height, const std::string& title);
	~Window();

	int shouldClose() const;
	void update();
	static float getCurrentTime();

	float getDelta() const { return m_Delta; }
	GLFWwindow* getGLFWWindow() const { return m_ID; }

	int getDisplayHeight() const { return m_DisplayHeight; }
	int getDisplayWidth() const { return m_DisplayWidth; }
	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }
};

