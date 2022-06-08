#pragma once

#include <iostream>
#include <unordered_map>

#include <GL/glew.h>

#if _WIN32
	#include <Windows.h>
	#define HANDLE GetStdHandle(STD_OUTPUT_HANDLE)
	#define RED SetConsoleTextAttribute(HANDLE, FOREGROUND_RED)
	#define ORANGE SetConsoleTextAttribute(HANDLE, FOREGROUND_RED | FOREGROUND_GREEN)
	#define YELLOW SetConsoleTextAttribute(HANDLE, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
	#define LIGHT_BLUE SetConsoleTextAttribute(HANDLE, FOREGROUND_BLUE  | FOREGROUND_INTENSITY)
	#define CLEAR SetConsoleTextAttribute(HANDLE, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#else
	#define RED
	#define ORANGE
	#define YELLOW
	#define LIGHT_BLUE
	#define CLEAR
#endif

#ifdef _MSC_VER
	#define ASSERT(x) if (!(x)) __debugbreak()
#else
	#include <cassert>
	#define ASSERT(x) if (!(x)) assert(false)
#endif

#define DEBUG

#ifdef DEBUG
#define GLAPI(x) Debug::clearGLError(); x; ASSERT(Debug::logGLFunc(#x, __FILE__, __LINE__))
#else
	#define GLAPI(x) x
#endif

namespace Debug {

	inline std::unordered_map<const char*, bool> supportedExtensions;

	void clearGLError();
	bool logGLFunc(const char* functionName, const char* errorFile, int lineNum);

	void checkExtensionsSupported();

	template<typename T>
	void checkExtensionsSupported(const T& extension) {
		const bool supported = glfwExtensionSupported(extension);

		supportedExtensions[extension] = supported;

		std::cout << extension << " : " << (supported ? "" : "NOT ") << "SUPPORTED" << std::endl;
	}

	template<typename First, typename ...Rest>
	void checkExtensionsSupported(const First& first, const Rest&... otherExtensions) {
		checkExtensionsSupported(first);
		checkExtensionsSupported(otherExtensions...);
	}

	void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

};