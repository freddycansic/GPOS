#include <iostream>
#include <stdexcept>

#include "Window.h"
#include "Debug.h"

bool Window::s_Initialised = false;
int Window::s_DisplayWidth = 0, Window::s_DisplayHeight = 0;

void Window::init() {
	// initialise GLFW
	if (!glfwInit()) {
		throw std::runtime_error("GLFW failed to initialise!");
	}

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// glfw core profile = no default vao, GLFW_OPENGL_COMPAT_PROFILE = there is a default vao

	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	s_DisplayWidth = vidmode->width;
	s_DisplayHeight = vidmode->height;

	s_Initialised = true;
}

Window::Window(const WindowConfig& config)
{
	if (!s_Initialised) {
		throw std::runtime_error("Window class not initialised! Did you call Window::init() first?");
	}

	// deduce window width and height
	if (config.maxSize) {
		m_Width = s_DisplayWidth;
		m_Height = s_DisplayHeight;	
	}
	else {
		m_Width = config.width;
		m_Height = config.height;
	}
	
	// init window
	m_ID = glfwCreateWindow(m_Width, m_Height, config.title.c_str(), NULL, NULL);
	
	if (!m_ID) {
		glfwTerminate();
		throw std::runtime_error("Failed to create window!");
	}

	// set opengl rendering context
	glfwMakeContextCurrent(m_ID);

	// vsync = true
	glfwSwapInterval(config.vsync);

	// initialise GLEW, must be called after there is a opengl rendering context
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("GLEW initialisation failed!");
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

int Window::getDisplayHeight() {
	return s_DisplayHeight;
}

int Window::getDisplayWidth() {
	return s_DisplayWidth;
}
