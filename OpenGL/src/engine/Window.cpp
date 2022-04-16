#include <GL/glew.h>
#include "Window.h"

#include <iostream>
#include "Debug.h"
#include "engine/rendering/opengl/Texture.h"

Window::Window(int width, int height, const std::string& title) 
	: m_Width(width), m_Height(height)
{
	// initialise GLFW
	if (!glfwInit())
		std::cout << "GLFW failed initialisation!" << std::endl;;

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// glfw core profile = no default vao, GLFW_OPENGL_COMPAT_PROFILE = there is a default vao


	m_ID = glfwCreateWindow(width, height, "Hello, world!", NULL, NULL);
	if (!m_ID) {
		glfwTerminate();
		std::cout << "Failed to create window!" << std::endl;;
	}
	
	// get display width + height
	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	m_DisplayWidth = vidmode->width;
	m_DisplayHeight = vidmode->height;

	// set opengl rendering context
	glfwMakeContextCurrent(m_ID);

	// vsync = true
	glfwSwapInterval(1);

	// initialise GLEW, must be called after there is a opengl rendering context
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW initialisation failed!" << std::endl;
		std::cin.get();
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(Debug::GLDebugMessageCallback, 0);

}

Window::~Window()
{
	// clean up resources
	glfwDestroyWindow(m_ID);
	glfwTerminate();

}

float Window::getCurrentTime() {
	return (float) glfwGetTime();
}

float lastTime = Window::getCurrentTime();

void Window::update() {
	// calculate deltatime
	float currentTime = getCurrentTime();
	m_Delta = currentTime - lastTime;
	lastTime = currentTime;

	glfwSwapBuffers(m_ID);
	glfwPollEvents();
}

int Window::shouldClose() const {
	return glfwWindowShouldClose(m_ID);
}
