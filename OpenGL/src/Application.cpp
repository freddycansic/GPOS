#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

void errorCallback(int error, const char* description)
{
	std::cerr << "Error: " << description << "";
}

int main(void)
{
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

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		// draw
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// destroy windows and release resources used by GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}