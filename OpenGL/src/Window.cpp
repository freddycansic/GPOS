#include "Window.h"

#include <iostream>

Window::Window(unsigned int width, unsigned int height, const std::string& title) 
	: m_Width(width), m_Height(height)
{
	// initialise GLFW
	if (!glfwInit())
		std::cout << "GLFW failed initialisation!" << std::endl;;

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfw core profile = no default vao, GLFW_OPENGL_COMPAT_PROFILE = there is a default vao

	m_ID = glfwCreateWindow(width, height, "Hello, world!", NULL, NULL);
	if (!m_ID) {
		glfwTerminate();
		std::cout << "Failed to create window!" << std::endl;;
	}

	// set opengl rendering context
	glfwMakeContextCurrent(m_ID);

	// vsync = true
	glfwSwapInterval(1);
}

Window::~Window()
{
	// clean up resources
	glfwDestroyWindow(m_ID);
	glfwTerminate();

}

void Window::update() {
	glfwSwapBuffers(m_ID);
	glfwPollEvents();
}

int Window::shouldClose() const {
	return glfwWindowShouldClose(m_ID);
}
