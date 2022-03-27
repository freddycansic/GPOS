#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Window.h"
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

int main(void)
{
	//Mat4 A(
	//	1, 2, 3, 4,
	//	5, 6, 7, 8,
	//	9, 10, 11, 12,
	//	13, 14, 15, 16
	//);

	//Mat4 B(
	//	2, 4, 6, 8,
	//	10, 12, 14, 16,
	//	18, 20, 22, 24,
	//	26, 28, 30, 32
	//);

	Window window(1024, 1024, "Hello, world!");

	// initialise GLEW, must be called after there is a opengl rendering context
	if (glewInit() != GLEW_OK)
		return -1;

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GLDebugMessageCallback, 0);

	//GLfloat vertices[] = {
	//	 0.5f,  0.5f, // top right
	//	-0.5f,  0.5f, // top left
	//	- 0.5f, -0.5f, // bottom left
	//	 0.5f, -0.5f// bottom right
	//};

	GLfloat vertices[] = {
		 0.5f,  0.5f, 1.0f, 1.0f, // top right
		-0.5f,  0.5f, 0.0f, 1.0f  // top left
		-0.5f, -0.5f, 0.0f, 0.0f, // bottom left
		 0.5f, -0.5f, 1.0f, 0.0f, // bottom right
	};

	constexpr unsigned int VERTEX_LENGTH = 4; // 4 floats per vertex
	constexpr unsigned int VERTICES_COUNT = VERTEX_LENGTH * 4; // 4 vertices total

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	constexpr unsigned int INDEX_BUFFER_COUNT = 6;

	VertexArray vao;
	VertexBuffer vbo(vertices, VERTICES_COUNT * sizeof(GLfloat));
	IndexBuffer ibo(indices, GL_UNSIGNED_INT, INDEX_BUFFER_COUNT);

	VertexBufferLayout layout;
	layout.addElement<GLfloat>(2, false);
	layout.addElement<GLfloat>(2, false);

	vao.addBuffer(vbo, layout);

	Shader shader("res/shaders/default.vert", "res/shaders/default.frag");
	shader.bind();

	Texture texture("res/textures/image.png");
	texture.bind(); // default no params = texture slot 0

	// texture is bound to slot 0
	shader.setUniform1i("u_Texture", 0);
	
	Renderer r;

	// unbind vao before ibo
	vao.unbind();
	shader.unbind();
	vbo.unbind();
	// so unbinding the ibo doesn't affect the vao
	ibo.unbind();

	float xTranslate = 0.0f, increment = 0.05f;

	float lastTime = glfwGetTime();
	while (!window.shouldClose()) {
		// calculate deltaTime
		float currentTime = glfwGetTime();
		float delta = currentTime - lastTime;
		lastTime = currentTime;	

		r.clear();
		
		if (xTranslate > 1.0f || xTranslate < -1.0f) increment *= -1;
		xTranslate += increment * delta * 50;

		// translation, rotation, scale function = scale, rotate, translate matrix
		Mat4 transform = Mat4::identity().translate(xTranslate, 0, 0).rotate(0, 0, currentTime).scale(0.3f);

		shader.bind();
		shader.setUniformMat4("u_Transform", transform);

		// draw
		r.draw(vao, ibo, shader);

		window.update();
	}

	return 0;
}