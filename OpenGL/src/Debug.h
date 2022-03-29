#pragma once

#include <GL/glew.h>
#include <iostream>

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

namespace Debug {

	void GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		const char* severityStr;
		const char* sourceStr;
		const char* typeStr;

		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			severityStr = "HIGH SEVERITY";
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			severityStr = "MEDIUM SEVERITY";
			break;

		case GL_DEBUG_SEVERITY_LOW:
			severityStr = "LOW SEVERITY";
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			severityStr = "NOTIFICATION";
			break;

		default:
			severityStr = "UNKNOWN SEVERITY";
		}

		switch (source) {
		case GL_DEBUG_SOURCE_API:
			sourceStr = "OPENGL API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			sourceStr = "WINDOW";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			sourceStr = "SHADER COMPILATION";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
			sourceStr = "THIRD PARTY";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:
			sourceStr = "APPLICATION";
			break;

			// GL_DEBUG_SOURCE_OTHER
		default:
			sourceStr = "UNKNOWN SOURCE";
		}

		switch (type) {
		case GL_DEBUG_TYPE_ERROR:
			typeStr = "ERROR";
			break;

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			typeStr = "DEPRECATED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			typeStr = "UNDEFINED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_PORTABILITY:
			typeStr = "PORTABILITY";
			break;

		case GL_DEBUG_TYPE_PERFORMANCE:
			typeStr = "PERFORMANCE";
			break;

		case GL_DEBUG_TYPE_MARKER:
			typeStr = "MARKER";
			break;

		case GL_DEBUG_TYPE_PUSH_GROUP:
			typeStr = "PUSH GROUP";
			break;

		case GL_DEBUG_TYPE_POP_GROUP:
			typeStr = "POP GROUP";
			break;

			// GL_DEBUG_TYPE_OTHER
		default:
			typeStr = "UNKNOWN TYPE";
		}

		// handle funny colours
		std::cout << "*";
		switch (severity) {

		case GL_DEBUG_SEVERITY_HIGH:
			RED;
			std::cout << severityStr;
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			ORANGE;
			std::cout << severityStr;
			break;

		case GL_DEBUG_SEVERITY_LOW:
			YELLOW;
			std::cout << severityStr;
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			LIGHT_BLUE;
			std::cout << severityStr;
			break;

		default:
			std::cout << severityStr;
		}
		CLEAR;

		std::cout << "* [" << sourceStr << "] (" << typeStr << "): " << message << std::endl;
	}

};
