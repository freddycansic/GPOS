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

void glfwError(int id, const char* description)
{
	std::cout << description << std::endl;
}

namespace Window
{
	void init(const WindowConfig& config)
	{
		glfwSetErrorCallback(glfwError);

		if (!glfwInit()) 
		{
			throw std::runtime_error("GLFW failed to initialise!");
		}

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);
		// GLFW_OPENGL_CORE_PROFILE = no default vao, GLFW_OPENGL_COMPAT_PROFILE = there is a default vao

		const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		s_DisplayWidth = vidmode->width;
		s_DisplayHeight = vidmode->height;

		// init window
		glfwWindowHint(GLFW_MAXIMIZED, config.maximised ? GLFW_TRUE : GLFW_FALSE);
		glfwWindowHint(GLFW_SAMPLES, config.samples);

		m_ID = glfwCreateWindow(config.width, config.height, config.title.c_str(), GLFW_FALSE, GLFW_FALSE);

		glfwGetWindowSize(m_ID, &m_Width, &m_Height);

		if (!m_ID)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create window!");
		}

		// set opengl rendering context
		glfwMakeContextCurrent(m_ID);

		// vsync
		glfwSwapInterval(config.vsync);

		// initialise GLEW, must be called after there is a opengl rendering context
		if (glewInit() != GLEW_OK) 
		{
			throw std::runtime_error("GLEW initialisation failed!");
		}

		GLAPI(glViewport(0, 0, m_Width, m_Height));

		// TODO need to move this
		GLAPI(glEnable(GL_DEBUG_OUTPUT));
		GLAPI(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
		glEnable(GL_MULTISAMPLE);

		//GLAPI(glEnable(GL_DEPTH_TEST));
		GLAPI(glDebugMessageCallback(Debug::GLDebugMessageCallback, nullptr));

		glfwSetCursorPosCallback(m_ID, Input::Callbacks::mouseCallback);
		glfwSetKeyCallback(m_ID, Input::Callbacks::keyCallback);
		glfwSetMouseButtonCallback(m_ID, Input::Callbacks::mouseButtonCallback);
		glfwSetFramebufferSizeCallback(m_ID, Input::Callbacks::frameBufferSizeCallback);
		glfwSetScrollCallback(m_ID, Input::Callbacks::scrollCallback);

		// turn off notifications
		GLAPI(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE));
	}

	void update()
	{
		static float lastTime = currentTime();

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

	void setHeight(int height) { m_Height = height; }
	void setWidth(int width) { m_Width = width; }
	int height() { return m_Height; }
	int width() { return m_Width; }

	GLFWwindow* GLFWWindow() { return m_ID;  };

	void destroy()
	{
		glfwDestroyWindow(m_ID);
		glfwTerminate();
	}

}