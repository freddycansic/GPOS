#include <iostream>
#include <stdexcept>

#include "Debug.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "engine/input/Input.h"

GLFWwindow* m_ID;
int m_Width, m_Height, s_DisplayWidth, s_DisplayHeight;
float m_Delta = 0;
bool s_Initialised;

namespace Window
{
	// setup window and opengl context
	void init(const WindowConfig& config)
	{
		// initialise GLFW
		if (!glfwInit()) {
			throw std::runtime_error("GLFW failed to initialise!");
		}

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		// GLFW_OPENGL_CORE_PROFILE = no default vao, GLFW_OPENGL_COMPAT_PROFILE = there is a default vao

		const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		s_DisplayWidth = vidmode->width;
		s_DisplayHeight = vidmode->height;

		s_Initialised = true;

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

		// need to move this
		GLAPI(glEnable(GL_DEBUG_OUTPUT));
		GLAPI(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));

		//GLAPI(glEnable(GL_DEPTH_TEST));
		GLAPI(glDebugMessageCallback(Debug::GLDebugMessageCallback, 0));
		GLAPI(glfwSetCursorPosCallback(Window::GLFWWindow(), Input::Callbacks::mouseCallback));
		GLAPI(glfwSetKeyCallback(Window::GLFWWindow(), Input::Callbacks::keyCallback));
		GLAPI(glfwSetMouseButtonCallback(Window::GLFWWindow(), Input::Callbacks::mouseButtonCallback));

		// turn off notifications
		GLAPI(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE));
	}

	float lastTime = currentTime();

	void update()
	{
		// calculate deltatime
		const float current = currentTime();
		m_Delta = current - lastTime;
		lastTime = current;

		glfwSwapBuffers(m_ID);
		glfwPollEvents();
	}

	int shouldClose() { return glfwWindowShouldClose(m_ID); }

	// probably some way I can simplify this logic
	bool wasCloseCalled = false;
	bool closeCalled() { return wasCloseCalled; }
	void close() { wasCloseCalled = true; }
	//

	bool m_Capturing = true;

	void beginCursorCapture()
	{
		glfwSetInputMode(m_ID, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_Capturing = true;
	}

	void endCursorCapture()
	{
		glfwSetInputMode(m_ID, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_Capturing = false;
	}

	bool capturingCursor() { return m_Capturing; }

	float currentTime() { return static_cast<float>(glfwGetTime()); }
	float deltatime() { return m_Delta;  }

	int displayHeight() { return s_DisplayHeight; }
	int displayWidth() { return s_DisplayWidth; }

	int height() { return m_Height; }
	int width() { return m_Width; }

	GLFWwindow* GLFWWindow() { return m_ID;  };

	void destroy()
	{
		// clean up resources
		glfwDestroyWindow(m_ID);
		glfwTerminate();
	}

}