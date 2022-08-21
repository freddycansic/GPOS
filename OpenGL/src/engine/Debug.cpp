#include "Debug.h"

#include <iostream>
#include <string>

const char* severityEnumToString(GLenum severity) {
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		return "HIGH SEVERITY";

	case GL_DEBUG_SEVERITY_MEDIUM:
		return "MEDIUM SEVERITY";

	case GL_DEBUG_SEVERITY_LOW:
		return "LOW SEVERITY";

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return "NOTIFICATION";

	default:
		return "UNKNOWN SEVERITY";
	}
}

const char* sourceEnumToString(GLenum source) {
	switch (source) {
	case GL_DEBUG_SOURCE_API:
		return "OPENGL API";

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return "WINDOW";

	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return "SHADER COMPILATION";

	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return "THIRD PARTY";

	case GL_DEBUG_SOURCE_APPLICATION:
		return "APPLICATION";

		// GL_DEBUG_SOURCE_OTHER
	default:
		return "UNKNOWN SOURCE";
	}
}

const char* typeEnumToString(GLenum type) {
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		return "ERROR";

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return "DEPRECATED BEHAVIOR";

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return "UNDEFINED BEHAVIOR";

	case GL_DEBUG_TYPE_PORTABILITY:
		return "PORTABILITY";

	case GL_DEBUG_TYPE_PERFORMANCE:
		return "PERFORMANCE";

	case GL_DEBUG_TYPE_MARKER:
		return "MARKER";

	case GL_DEBUG_TYPE_PUSH_GROUP:
		return "PUSH GROUP";

	case GL_DEBUG_TYPE_POP_GROUP:
		return "POP GROUP";

		// GL_DEBUG_TYPE_OTHER
	default:
		return "UNKNOWN TYPE";
	}
}

namespace Debug {
	
	void clearGLError() {
		while (glGetError() != GL_NO_ERROR);
	}

	bool logGLFunc(const char* functionName, const char* errorFile, int lineNum) {
		while (const GLenum error = glGetError()) {
			std::string filePathFull(errorFile);
			
			const std::string fileName = filePathFull.substr(filePathFull.find_last_of('\\')+1);
			const std::string filePath = filePathFull.substr(0, filePathFull.find(fileName));

			std::cout << "(" << error << ") " << filePath;
			YELLOW;
			std::cout << fileName;
			CLEAR;
			std::cout << " " << functionName << " : " << lineNum << std::endl;
			return false;
		}
		return true;
	}

	void checkExtensionsSupported() {}

	void checkExtensionsSupported(const char* extension) {
		const bool supported = glfwExtensionSupported(extension);

		supportedExtensions[extension] = supported;

		std::cout << extension << " : " << (supported ? "" : "NOT ") << "SUPPORTED" << std::endl;
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