#include "Shader.h"

#include <fstream>
#include <sstream>

std::ostream& operator<<(std::ostream& os, const std::unordered_map<std::string, int>& map) {
	for (const auto& element : map) {
		std::string key = element.first;
		int value = element.second;

		os << "Name = " << key << " Location = " << value << "\n";
	}
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

		if (token == "uniform") {
			std::string uniformName = tokens[i + 2];

			if (uniformName.at(uniformName.size() - 1) == ';')
				uniformName = uniformName.substr(0, uniformName.size() - 1);

			std::cout << "Found uniform " << uniformName << std::endl;

			int uniformLocation = glGetUniformLocation(m_ID, uniformName.c_str());
			
			if (uniformLocation == -1) {
				std::cout << "Uniform not in use." << std::endl;
			}
			
			m_Uniforms[uniformName] = uniformLocation;
		}
	}

}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	if (m_Uniforms.count(name) == 0) std::cout << "Uniform not found in shader!" << std::endl;
	glUniform4f(m_Uniforms[name], v0, v1, v2, v3);
	// unordered map operator[] is O(1) so this is fine
}

void Shader::setUniformMat4(const std::string& name, const Mat4& matrix) {
	if (m_Uniforms.count(name) == 0) std::cout << "Uniform not found in shader!" << std::endl;
	glUniformMatrix4fv(m_Uniforms[name], 1, GL_TRUE, matrix.getPtr());
}

void Shader::setUniform1i(const std::string& name, int value) {
	if (m_Uniforms.count(name) == 0) std::cout << "Uniform " << name << " does not exist" << std::endl;
	glUniform1i(m_Uniforms[name], value);
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