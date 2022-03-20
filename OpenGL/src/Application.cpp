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

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		// triangle
		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, (float) sqrt(3) / 2);
		glVertex2f(0.5f, -0.5f);
		glEnd();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// destroy windows and release resources used by GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}