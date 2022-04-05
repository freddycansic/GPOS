#pragma once

#include <GLFW/glfw3.h>
#include <string>

class Window
{
private:
	GLFWwindow* m_ID;
	unsigned int m_Width, m_Height;
	float m_Delta;
public:

	Window(unsigned int width, unsigned int height, const std::string& title);
	~Window();

	int shouldClose() const;
	void update();
	static float getCurrentTime();

	float getDelta() const { return m_Delta; }
	GLFWwindow* getGLFWWindow() const { return m_ID; }
};

