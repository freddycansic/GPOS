#include "Shader.h"

#include <fstream>
#include <sstream>

std::ostream& operator<<(std::ostream& os, const Uniform& uniform) {
	os << "Name = \"" << uniform.name << "\" Location = " << uniform.location;
	return os;
}

std::string getFileContents(const std::string& filepath) {

	std::ostringstream stream;
	stream << std::ifstream(filepath).rdbuf();

	return stream.str();
}

void Shader::findAndAddUniforms(const std::string& source) {
	
	// tokenize source
	std::vector<std::string> tokens;
	std::stringstream ss(source);

	std::string token;
	while (ss >> token) {
		tokens.push_back(token);
	}

	for (unsigned int i = 0; i < tokens.size(); i++) {
		std::string token = tokens[i];

		if (token == "uniform") {					// remove semi-colon at the end of token
			std::string uniformName = tokens[i + 2].substr(0, tokens[i+2].size()-1);
			m_Uniforms.push_back({ uniformName, glGetUniformLocation(m_ID, uniformName.c_str()) });
		}
	}

}

unsigned int compileShader(unsigned int type, const std::string& source) {
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

unsigned int createProgram(const std::string& vertexSource, const std::string& fragmentSource) {

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

Shader::Shader(const std::string& vertexShaderSourceDir, const std::string& fragmentShaderSourceDir) {
	std::string vertexShaderSource = getFileContents(vertexShaderSourceDir);
	std::string fragmentShaderSource = getFileContents(fragmentShaderSourceDir);

	m_ID = createProgram(vertexShaderSource, fragmentShaderSource);

	findAndAddUniforms(vertexShaderSource);
	findAndAddUniforms(fragmentShaderSource);

	for (const auto& uniform : m_Uniforms) {
		std::cout << uniform << std::endl;
	}
}

void Shader::bind() const {
	glUseProgram(m_ID);
}

void Shader::unbind() const {
	glUseProgram(0);
}

Shader::~Shader() {
	glDeleteProgram(m_ID);
}