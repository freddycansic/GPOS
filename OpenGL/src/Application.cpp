#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <string>

#include "maths/Mat4.h"

void errorCallback(int error, const char* description)
{
	std::cerr << "Error: " << description << "";
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
	Mat4 matrix(
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 14, 15, 16
	);

	Mat4 matrix2(
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 14, 15, 16
	);

	

	GLFWwindow* window;

	// initialise GLFW
	if (!glfwInit())
		return -1;

	// get vidmode of primary monitor
	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	// create window + context, set to 3/4 screen width + height
	window = glfwCreateWindow(3 * vidmode->width / 4, 3 * vidmode->height / 4, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// callbacks
	glfwSetErrorCallback(errorCallback);

	// set opengl rendering context
	glfwMakeContextCurrent(window);

	// initialise GLEW, must be called after there is a opengl rendering context
	if (glewInit() != GLEW_OK)
		return -1;

	float vertices[3 * 2] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	// gen vertex buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	// bind it
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// push data into it
	glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float), vertices, GL_STATIC_DRAW);

	// setup vertex attribs:
	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);

	std::string vertexSource =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main() {\n"
		"	gl_Position = position;\n"
		"}\n";

	std::string fragmentSource = 
		"#version 330 core\n"
		"\n"
		"out vec4 color;\n"
		"\n"
		"void main() {\n"
		"	color = vec4(1.0, 0.0, 1.0, 1.0);\n"
		"}\n";

	unsigned int shaderProgram = createProgram(vertexSource, fragmentSource);
	glUseProgram(shaderProgram);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		// draw
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// clean up resources
	glDeleteBuffers(1, &buffer);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}