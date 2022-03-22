#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "maths/Mat4.h"

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

	std::cout << "A = " << A << std::endl;
	std::cout << "B = " << B << std::endl;

	A = std::move(B);

	std::cout << "A = " << A << std::endl;
	std::cout << "B = " << B << std::endl;

	
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

	// set opengl rendering context
	glfwMakeContextCurrent(window);

	// initialise GLEW, must be called after there is a opengl rendering context
	if (glewInit() != GLEW_OK)
		return -1;

	std::vector<float> vertices = {
		-0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f
	};

	// gen vertex buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	// bind it
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// push data into it
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// setup vertex attribs:
	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(2 * sizeof(float)));

	std::string vertexSource = getFileContents("res/shaders/default.vert");
	std::string fragmentSource = getFileContents("res/shaders/default.frag");

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