#include "Shader.h"

#include <fstream>
#include <sstream>
#include "engine/Debug.h"

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

	// create vector of all tokens
	std::string token;
	while (ss >> token) {
		tokens.push_back(token);
	}

	// iterate over tokens
	for (unsigned int i = 0; i < tokens.size(); i++) {
		std::string token = tokens[i];

		// if we find a uniform
		if (token == "uniform") {
			// then we know the identifier of that uniform will be 2 tokens after it
			std::string uniformName = tokens[i + 2];

			// remove array notation e.g "[4]"
			size_t startBracketIndex = uniformName.find('[');
			if (startBracketIndex != std::string::npos) {
				uniformName = uniformName.substr(0, startBracketIndex);
			}
			
			// remove semi colon at end
			if (uniformName.at(uniformName.size() - 1) == ';')
				uniformName = uniformName.substr(0, uniformName.size() - 1);

			int uniformLocation = glGetUniformLocation(m_ID, uniformName.c_str());
			
			if (uniformLocation == -1) {
				std::cout << "Uniform not in use." << std::endl;
			}
			
			// insert the uniform into a map
			m_Uniforms[uniformName] = uniformLocation;
		}
	}

}

void Shader::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4) const {
	if (m_Uniforms.count(name) == 0) std::cout << "Uniform not found in shader!" << std::endl;
	GLAPI(glUniform4f(m_Uniforms.at(name), v1, v2, v3, v4));
	// unordered map operator[] is O(1) so this is fine
}

void Shader::setUniformMat4(const std::string& name, const Mat4& matrix) const {
	if (m_Uniforms.count(name) == 0) std::cout << "Uniform not found in shader!" << std::endl;
	GLAPI(glUniformMatrix4fv(m_Uniforms.at(name), 1, GL_TRUE, matrix.getPtr()));
}

void Shader::setUniform1i(const std::string& name, int value) const {
	if (m_Uniforms.count(name) == 0) std::cout << "Uniform " << name << " does not exist" << std::endl;
	GLAPI(glUniform1i(m_Uniforms.at(name), value));
}

void Shader::setUniform1iv(const std::string& name, size_t count, const int* value) const {
	if (m_Uniforms.count(name) == 0) std::cout << "Uniform " << name << " does not exist" << std::endl;
	GLAPI(glUniform1iv(m_Uniforms.at(name), (GLsizei) count, value));
}

unsigned int compileShader(unsigned int type, const std::string& source) {
	// generate shader
	unsigned int id = glCreateShader(type);

	// link shader with its source code
	const char* charSource = source.c_str();
	GLAPI(glShaderSource(id, 1, &charSource, nullptr));

	// compile shader with source code into executable program to be run on gpu
	GLAPI(glCompileShader(id));

	// store value of GL_COMPILE_STATUS in result
	int result;
	GLAPI(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) { // if it failed
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl;

		// get length of the shader info log
		int length;
		GLAPI(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// allocate block of memory on the STACK dynamically, then cast to char pointer
		char* message = (char*)alloca(length * sizeof(char));
		GLAPI(glGetShaderInfoLog(id, length, &length, message));

		std::cout << message << std::endl;

		return 0;
	}

	return id;
}

unsigned int createProgram(const std::string& vertexSource, const std::string& fragmentSource) {

	// generate program
	unsigned int program = glCreateProgram());

	// generate shaders
	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	// attach shaders to program
	GLAPI(glAttachShader(program, vertexShader));
	GLAPI(glAttachShader(program, fragmentShader));

	// link program + error check
	GLAPI(glLinkProgram(program));

	int result;
	GLAPI(glGetProgramiv(program, GL_LINK_STATUS, &result));
	if (result == GL_FALSE) {
		std::cout << "Failed to link program" << std::endl;

		int length;
		GLAPI(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));

		char* message = (char*)alloca(length * sizeof(char));
		GLAPI(glGetProgramInfoLog(program, length, &length, message));

		std::cout << message << std::endl;

		return 0;
	}

	// validate program + error check
	GLAPI(glValidateProgram(program));

	GLAPI(glGetProgramiv(program, GL_VALIDATE_STATUS, &result));
	if (result == GL_FALSE) {
		std::cout << "Failed to validate program" << std::endl;

		int length;
		GLAPI(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));

		char* message = (char*)alloca(length * sizeof(char));
		GLAPI(glGetProgramInfoLog(program, length, &length, message));

		std::cout << message << std::endl;

		return 0;
	}

	// delete shaders as they are no longer needed as they are contained in the program
	GLAPI(glDeleteShader(vertexShader));
	GLAPI(glDeleteShader(fragmentShader));

	return program;
}

Shader::Shader(const std::string& vertexShaderSourceDir, const std::string& fragmentShaderSourceDir) {
	std::string vertexShaderSource = getFileContents(vertexShaderSourceDir);
	std::string fragmentShaderSource = getFileContents(fragmentShaderSourceDir);

	m_ID = createProgram(vertexShaderSource, fragmentShaderSource);

	findAndAddUniforms(vertexShaderSource);
	findAndAddUniforms(fragmentShaderSource);
}

Shader::Shader() :
	m_ID(0)
{
}

void Shader::bind() const {
	GLAPI(glUseProgram(m_ID));
}

void Shader::unbind() const {
	GLAPI(glUseProgram(0));
}

Shader::~Shader() {
	GLAPI(glDeleteProgram(m_ID));
}