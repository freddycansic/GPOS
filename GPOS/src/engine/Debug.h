#pragma once

#include <iostream>
#include <unordered_map>

#include <GL/glew.h> // TODO fix this
#include <GLFW/glfw3.h>

#ifdef _MSC_VER
	#define ASSERT_WITH_MSG(x, msg) if (!(x)) { std::cout << (msg) << std::endl; __debugbreak();}
#else
	#include <cassert>
	#define ASSERT_WITH_MSG(x, msg) if (!(x)) { std::cout << (msg) << std::endl; assert(false);}
#endif

#define ASSERT(x) ASSERT_WITH_MSG(x, "")

#ifdef DEBUG
	#define GLAPI(x) Debug::clearGLError(); x; ASSERT(Debug::logGLFunc(#x, __FILE__, __LINE__))
#else
	#define GLAPI(x) x
#endif

namespace Debug
{
	inline std::unordered_map<const char*, bool> supportedExtensions;

	void clearGLError();
	bool logGLFunc(const char* functionName, const char* errorFile, int lineNum);

	void checkExtensionsSupported();
	void checkExtensionsSupported(const char* extension);

	template<typename First, typename ...Rest>
	void checkExtensionsSupported(const First& first, const Rest&... otherExtensions) {
		checkExtensionsSupported(first);
		checkExtensionsSupported(otherExtensions...);
	}

	void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

};