#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "maths/Mat4.h"

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

static std::string getFileContents(const std::string& filepath) {
	
	std::ostringstream stream;
	stream << std::ifstream(filepath).rdbuf();

	return stream.str();
}

static unsigned int compileShader(unsigned int type, const std::string& source) {
	// generate shader
	unsigned int id = glCreateShader(type);
	
	// link shader with its source code
	const char* charSource = source.c_str();
	glShaderSource(id, 1, &charSource, nullptr);
	
	// compile shader with source code into executable program to be run on gpu
	glCompileShader(id);

	// store value of GL_COMPILE_STATUS in result
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) { // if it failed
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl;
		
		// get length of the shader info log
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		
		// allocate block of memory on the STACK dynamically, then cast to char pointer
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		
		std::cout << message << std::endl;

		return 0;
	}

	return id;
}

static unsigned int createProgram(const std::string& vertexSource, const std::string& fragmentSource) {

	// generate program
	unsigned int program = glCreateProgram();
	
	// generate shaders
	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	// attach shaders to program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	
	// link program + error check
	glLinkProgram(program);
	
	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		std::cout << "Failed to link program" << std::endl;

		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(program, length, &length, message);

		std::cout << message << std::endl;

		return 0;
	}

	// validate program + error check
	glValidateProgram(program);

	glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
	if (result == GL_FALSE) {
		std::cout << "Failed to validate program" << std::endl;

		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(program, length, &length, message);

		std::cout << message << std::endl;

		return 0;
	}

	// delete shaders as they are no longer needed as they are contained in the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

int main(void)
{
	Mat4 A(
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 14, 15, 16
	);

	Mat4 B(
		2, 4, 6, 8,
		10, 12, 14, 16,
		18, 20, 22, 24,
		26, 28, 30, 32
	);

	GLFWwindow* window;

	// initialise GLFW
	if (!glfwInit())
		return -1;

	// get vidmode of primary monitor
	//const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	// create window + context, set to 3/4 screen width + height
	//window = glfwCreateWindow(3 * vidmode->width / 4, 3 * vidmode->height / 4, "Hello World", NULL, NULL);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfw core profile = no default vao, GLFW_OPENGL_COMPAT_PROFILE = there is a default vao

	window = glfwCreateWindow(1024, 1024, "Hello, world!", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// set opengl rendering context
	glfwMakeContextCurrent(window);

	// vsync = true
	glfwSwapInterval(1);

	// initialise GLEW, must be called after there is a opengl rendering context
	if (glewInit() != GLEW_OK)
		return -1;

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GLDebugMessageCallback, 0);

	constexpr unsigned int VERTEX_LENGTH = 2;
	constexpr unsigned int VERTEX_BUFFER_LENGTH = VERTEX_LENGTH * 4;

	float vertices[] = {
		 0.5f,  0.5f, // top right
		-0.5f,  0.5f, // top left
		-0.5f, -0.5f, // bottom left
		 0.5f, -0.5f  // bottom right
	};

	constexpr unsigned int INDEX_BUFFER_COUNT = 6;

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	// vertex array object
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// index buffer
	IndexBuffer ibo(indices, INDEX_BUFFER_COUNT);

	VertexBuffer vbo(vertices, VERTEX_BUFFER_LENGTH * sizeof(GLfloat));

	// setup vertex attribs:
	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, VERTEX_LENGTH * sizeof(GLfloat), (const void*)0);

	std::string vertexSource = getFileContents("res/shaders/default.vert");
	std::string fragmentSource = getFileContents("res/shaders/default.frag");

	unsigned int shaderProgram = createProgram(vertexSource, fragmentSource);
	glUseProgram(shaderProgram);

	int uColorLocation = glGetUniformLocation(shaderProgram, "u_Color");
	glUniform4f(uColorLocation, 1.0f, 0.0f, 1.0f, 1.0f);

	// unbind vao before ibo
	glBindVertexArray(0);
	glUseProgram(0);
	vbo.unbind();
	// so unbinding the ibo doesn't affect the vao
	ibo.unbind();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		
		glBindVertexArray(vao);

		// draw
		glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// clean up resources
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}