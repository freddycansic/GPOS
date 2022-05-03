#include "Application.h"

#include "engine/Debug.h"

#include <fstream>
#include <sstream>

std::string readFile(const std::string& filepath) {
	std::ostringstream stream;
	stream << std::ifstream(filepath).rdbuf();

	return stream.str();
}

void Application::init() {
	vertices = {
		// positions			texture coords
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,		1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f
	};

	translations.reserve(100);
	for (float i = -1.5 * 5; i < 1.5 * 5; i+=1.5) {
		for (float j = -1.5 * 5; j < 1.5 * 5; j+=1.5) {
			translations.push_back(i);
			translations.push_back(j);
			translations.push_back(j > 0 ? 1 : 2); // texId
		}
	}

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
	GLAPI(glShaderSource(vertexShader, 1, &vertexCharSource, nullptr));
	GLAPI(glCompileShader(vertexShader));

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLAPI(glShaderSource(fragmentShader, 1, &fragmentCharSource, nullptr));
	GLAPI(glCompileShader(fragmentShader));

	GLAPI(glAttachShader(program, vertexShader));
	GLAPI(glAttachShader(program, fragmentShader));
	GLAPI(glLinkProgram(program));
	GLAPI(glValidateProgram(program));

	GLAPI(glDeleteShader(vertexShader));
	GLAPI(glDeleteShader(fragmentShader));

	unsigned int texID;
	uint64_t texHandle;

	GLAPI(glGenTextures(1, &texID));
	GLAPI(glBindTexture(GL_TEXTURE_2D, texID));

	GLAPI(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLAPI(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLAPI(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); // horizontal wrap
	GLAPI(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); // vertical wrap

	texHandle = glGetTextureHandleARB(texID);
	GLAPI(glMakeTextureHandleResidentARB(texHandle));

	unsigned int container;
	GLAPI(glGenBuffers(1, &container));
	GLAPI(glBindBuffer(GL_UNIFORM_BUFFER, container));
	GLAPI(glBufferData(GL_UNIFORM_BUFFER, sizeof(uint64_t), &texHandle, GL_STATIC_DRAW));

	GLAPI(glGenVertexArrays(1, &vao));
	GLAPI(glBindVertexArray(vao));

	GLAPI(glGenBuffers(1, &ivbo));
	GLAPI(glBindBuffer(GL_ARRAY_BUFFER, ivbo));
	GLAPI(glBufferData(GL_ARRAY_BUFFER, translations.size() * sizeof(float), translations.data(), GL_STATIC_DRAW));
	GLAPI(glBindBuffer(GL_ARRAY_BUFFER, 0));

	GLAPI(glGenBuffers(1, &vbo));
	GLAPI(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLAPI(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW));
	GLAPI(glBindBuffer(GL_ARRAY_BUFFER, 0));

	GLAPI(glGenBuffers(1, &ibo));
	GLAPI(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLAPI(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));
	GLAPI(glBindBuffer(GL_ARRAY_BUFFER, 0));

	GLAPI(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLAPI(glEnableVertexAttribArray(0));
	GLAPI(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)0));
	GLAPI(glEnableVertexAttribArray(1));
	GLAPI(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float))));

	GLAPI(glBindBuffer(GL_ARRAY_BUFFER, ivbo));
	GLAPI(glEnableVertexAttribArray(2));
	GLAPI(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0));
	GLAPI(glVertexAttribDivisor(2, 1));
	GLAPI(glEnableVertexAttribArray(3));
	GLAPI(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)(2 * sizeof(float))));
	GLAPI(glVertexAttribDivisor(3, 1));

	// TODO uniform buffer

	u_VPLoc = glGetUniformLocation(program, "u_ViewProj");
}

const Mat4 persp = Mat4::perspective(45.0f, 1920.0f / 1080.0f, 1.0f, 1000.0f);

void Application::render() {
	Renderer::clear(0.42f, 0.42f, 0.42f);

	Renderer::setViewMatrix(Mat4::identity.rotate(-viewTransform.rot.x, -viewTransform.rot.y, viewTransform.rot.z).translate(viewTransform.tra.x, viewTransform.tra.y, viewTransform.tra.z).scale(viewTransform.sca.x, viewTransform.sca.y, viewTransform.sca.z));

	GLAPI(glBindVertexArray(vao));
	GLAPI(glUseProgram(program));

	GLAPI(glUniformMatrix4fv(u_VPLoc, 1, GL_TRUE, (persp * Mat4::identity.rotate(-viewTransform.rot.x, -viewTransform.rot.y, viewTransform.rot.z).translate(viewTransform.tra.x, viewTransform.tra.y, viewTransform.tra.z).scale(viewTransform.sca.x, viewTransform.sca.y, viewTransform.sca.z)).getPtr()));

	GLAPI(glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr, translations.size() / 2));
}

void Application::imGuiRender() {
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::Begin("Debug", (bool*)1, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
	
	ImGui::Text("View");
	
	ImGui::Text("Translation");
	ImGui::SliderFloat("##10", &viewTransform.tra.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##11", &viewTransform.tra.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##12", &viewTransform.tra.z, -1000.0f, 100.0f);
							
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