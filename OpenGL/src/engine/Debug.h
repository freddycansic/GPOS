#pragma once

#include <GL/glew.h>

#define DEBUG

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

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef DEBUG
#define GLAPI(x) Debug::clearGLError(); x; ASSERT(Debug::logGLFunc(#x, __FILE__, __LINE__))
#else
	#define GLAPI(x) x
#endif

namespace Debug {

	void clearGLError();

	bool logGLFunc(const char* functionName, const char* fileName, int lineNum);

	void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

};