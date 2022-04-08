#pragma once

#include <GLFW/glfw3.h>
#include <string>

class Window
{
private:
	GLFWwindow* m_ID;
	unsigned int m_Width, m_Height, m_DisplayWidth, m_DisplayHeight;
	float m_Delta = 0;
public:

	Window(unsigned int width, unsigned int height, const std::string& title);
	~Window();

	int shouldClose() const;
	void update();
	static float getCurrentTime();

	float getDelta() const { return m_Delta; }
	GLFWwindow* getGLFWWindow() const { return m_ID; }

	unsigned int getDisplayHeight() const { return m_DisplayHeight; }
	unsigned int getDisplayWidth() const { return m_DisplayWidth; }
	unsigned int getWidth() const { return m_Width; }
	unsigned int getHeight() const { return m_Height; }
};

