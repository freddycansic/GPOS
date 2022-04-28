#include "Application.h"

#include <fstream>
#include <sstream>

std::string readFile(const std::string& filepath) {
	std::ostringstream stream;
	stream << std::ifstream(filepath).rdbuf();

	return stream.str();
}

void Application::init() {
	vertices = {
		// positions			colours						texture coords	texture id
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		-1.0f,
		-0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		-1.0f,
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		-1.0f,
		 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f,		-1.0f
	};

	indices = {
		0, 1, 2,
		0, 2, 3
	};

	program = glCreateProgram();
	
	std::string vertexSource = readFile("res/shaders/default.vert");
	std::string fragmentSource = readFile("res/shaders/default.frag");

	const char* vertexCharSource = vertexSource.c_str();
	const char* fragmentCharSource = fragmentSource.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCharSource, nullptr);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCharSource, nullptr);
	glCompileShader(fragmentShader);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(vao, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)0);
	glEnableVertexArrayAttrib(vao, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)(3 * sizeof(float)));
	glEnableVertexArrayAttrib(vao, 2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)(7 * sizeof(float)));
	glEnableVertexArrayAttrib(vao, 3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (const void*)(9 * sizeof(float)));

}

void Application::render() {
	Renderer::clear(0.42f, 0.42f, 0.42f);

	Renderer::setViewMatrix(Mat4::identity.rotate(-viewTransform.rot.x, -viewTransform.rot.y, viewTransform.rot.z).translate(viewTransform.tra.x, viewTransform.tra.y, viewTransform.tra.z).scale(viewTransform.sca.x, viewTransform.sca.y, viewTransform.sca.z));

	glBindVertexArray(vao);
	glUseProgram(program);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Application::imGuiRender() {
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::Begin("Debug", (bool*)1, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
	
	ImGui::Text("View");
	
	ImGui::Text("Translation");
	ImGui::SliderFloat("##10", &viewTransform.tra.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##11", &viewTransform.tra.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##12", &viewTransform.tra.z, -100.0f, 100.0f);
							
	ImGui::Text("Rotation");
	ImGui::SliderFloat("##13", &viewTransform.rot.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##14", &viewTransform.rot.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##15", &viewTransform.rot.z, -100.0f, 100.0f);
							
	ImGui::Text("Scale");	
	ImGui::SliderFloat("##16", &viewTransform.sca.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##17", &viewTransform.sca.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##18", &viewTransform.sca.z, -100.0f, 100.0f);

	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	
	ImGui::End();
}

void Application::destroy() {

}