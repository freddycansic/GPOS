#include "Debug.h"

const char* severityEnumToString(GLenum severity) {
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		return "HIGH SEVERITY";
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		return "MEDIUM SEVERITY";
		break;

	case GL_DEBUG_SEVERITY_LOW:
		return "LOW SEVERITY";
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return "NOTIFICATION";
		break;

	default:
		return "UNKNOWN SEVERITY";
	}
}

const char* sourceEnumToString(GLenum source) {
	switch (source) {
	case GL_DEBUG_SOURCE_API:
		return "OPENGL API";
		break;

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return "WINDOW";
		break;

	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return "SHADER COMPILATION";
		break;

	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return "THIRD PARTY";
		break;

	case GL_DEBUG_SOURCE_APPLICATION:
		return "APPLICATION";
		break;

		// GL_DEBUG_SOURCE_OTHER
	default:
		return "UNKNOWN SOURCE";
	}
}

const char* typeEnumToString(GLenum type) {
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		return "ERROR";
		break;

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return "DEPRECATED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return "UNDEFINED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_PORTABILITY:
		return "PORTABILITY";
		break;

	case GL_DEBUG_TYPE_PERFORMANCE:
		return "PERFORMANCE";
		break;

	case GL_DEBUG_TYPE_MARKER:
		return "MARKER";
		break;

	case GL_DEBUG_TYPE_PUSH_GROUP:
		return "PUSH GROUP";
		break;

	case GL_DEBUG_TYPE_POP_GROUP:
		return "POP GROUP";
		break;

		// GL_DEBUG_TYPE_OTHER
	default:
		return "UNKNOWN TYPE";
	}
}

namespace Debug {

	void clearGLError() {
		while (glGetError() != GL_NO_ERROR);
	}

	bool logGLFunc(const char* functionName, const char* fileName, int lineNum) {
		while (GLenum error = glGetError()) {
			std::cout << "ERROR! (" << error << ") " << fileName << " " << functionName << " : " << lineNum << std::endl;
			return false;
		}
		return true;
	}

	void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		const char* severityStr = severityEnumToString(severity);
		const char* sourceStr = sourceEnumToString(source);
		const char* typeStr = typeEnumToString(type);

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

}