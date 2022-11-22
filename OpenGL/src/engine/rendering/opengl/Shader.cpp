#include "Shader.h"

#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>

#include "engine/Debug.h"
#include "maths/Maths.h"

#include "maths/Vectors.h"

std::ostream& operator<<(std::ostream& os, const std::unordered_map<std::string, int>& map) {
	for (const auto& [name, location] : map) {
		os << "Name = " << name << " Location = " << location << "\n";
	}
	return os;
}

std::string getFileContents(const std::string& filepath) {

	std::ostringstream stream;
	stream << std::ifstream(filepath).rdbuf();

	return stream.str();
}

int Shader::getUniformLocation(const std::string& name)
{
	if (!m_Uniforms.contains(name))
	{
		const auto uniformLocation = glGetUniformLocation(m_ID, name.c_str());

		ASSERT_WITH_MSG(uniformLocation != -1, "Uniform not used / does not exist in shader!");

		m_Uniforms[name] = uniformLocation;
	}

	return m_Uniforms[name];
}

//void Shader::findAndAddUniforms(const std::string& filepath) {
//	
//	// tokenize source
//	std::vector<std::string> tokens;
//	std::stringstream ss(filepath);
//
//	// create vector of all tokens
//	std::string token;
//	while (ss >> token) {
//		tokens.push_back(token);
//	}
//
//	// iterate over tokens
//	for (unsigned int i = 0; i < tokens.size(); i++) {
//		const auto& token = tokens[i];
//
//		// if we find a uniform
//		if (token == "uniform") {
//			// then we know the identifier of that uniform will be 2 tokens after it
//			std::string uniformName = tokens[i + 2];
//
//			// remove array notation e.g "[4]"
//			const size_t startBracketIndex = uniformName.find('[');
//			if (startBracketIndex != std::string::npos) {
//				uniformName = uniformName.substr(0, startBracketIndex);
//			}
//
//			// remove semi colon at end
//			if (uniformName.at(uniformName.size() - 1) == ';')
//				uniformName = uniformName.substr(0, uniformName.size() - 1);
//			
//			const int uniformLocation = getUniformLocation(uniformName);
//
//			if (uniformLocation == -1) 
//			{
//				std::cout << "Uniform " << uniformName << " not in use." << std::endl;
//			}
//
//			// insert the uniform into a map
//			m_Uniforms[uniformName] = uniformLocation;
//		}
//	}
//}

void Shader::setUniform3f(const std::string& name, float v1, float v2, float v3)
{
	GLAPI(glUniform3f(getUniformLocation(name), v1, v2, v3));
}

void Shader::setUniform3f(const std::string& name, const Vec3& vec)
{
	setUniform3f(name, vec.x, vec.y, vec.z);
}

void Shader::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
{
	GLAPI(glUniform4f(getUniformLocation(name), v1, v2, v3, v4));
}

void Shader::setUniform4f(const std::string& name, const Vec4& vec)
{
	setUniform4f(name, vec.x, vec.y, vec.z, vec.w);
}

void Shader::setUniformMat4(const std::string& name, const Mat4x4& matrix)
{
	GLAPI(glUniformMatrix4fv(getUniformLocation(name), 1, GL_TRUE, matrix.data()));
}

void Shader::setUniform1i(const std::string& name, int value)
{
	GLAPI(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1ui64(const std::string& name, uint64_t value)
{

	//if (Debug::supportedExtensions.at("GL_ARB_gpu_shader_int64")) {
	//	GLAPI(glUniform1ui64ARB(m_Uniforms.at(name), value));
	//} else {
		// if we cant send 64 bits in one go send them in 2 halves by bit shifting
		GLAPI(glUniform2ui(getUniformLocation(name), static_cast<GLuint>(value), static_cast<GLuint>(value >> 32)));
	//}
}

void Shader::setUniform1iv(const std::string& name, size_t count, const int* value)
{
	GLAPI(glUniform1iv(getUniformLocation(name), static_cast<GLsizei>(count), value));
}

unsigned int compileShader(unsigned int type, const std::string& source)
{
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

	// if it failed
	if (result == GL_FALSE) 
	{ 
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl;

		// get length of the shader info log
		int length;
		GLAPI(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// allocate block of memory on the STACK dynamically, then cast to char pointer
		char* message = static_cast<char*>(alloca(length * sizeof(char)));
		GLAPI(glGetShaderInfoLog(id, length, &length, message));

		std::cout << message << std::endl;

		return 0;
	}

	return id;
}

unsigned int createProgram(const std::string& vertexSource, const std::string& fragmentSource)
{

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
	if (result == GL_FALSE) 
	{
		std::cout << "Failed to link program" << std::endl;

		int length;
		GLAPI(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));

		char* message = static_cast<char*>(alloca(length * sizeof(char)));
		GLAPI(glGetProgramInfoLog(program, length, &length, message));

		std::cout << message << std::endl;

		return 0;
	}

	// validate program + error check
	GLAPI(glValidateProgram(program));

	GLAPI(glGetProgramiv(program, GL_VALIDATE_STATUS, &result));
	if (result == GL_FALSE)
	{
		std::cout << "Failed to validate program" << std::endl;

		int length;
		GLAPI(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));

		char* message = static_cast<char*>(alloca(length * sizeof(char)));
		GLAPI(glGetProgramInfoLog(program, length, &length, message));

		std::cout << message << std::endl;

		return 0;
	}

	// delete shaders as they are no longer needed as they are contained in the program
	GLAPI(glDeleteShader(vertexShader));
	GLAPI(glDeleteShader(fragmentShader));

	return program;
}

Shader::Shader(const std::string& vertexShaderSourceDir, const std::string& fragmentShaderSourceDir)
{
	const auto vertexShaderSource = getFileContents(vertexShaderSourceDir);
	const auto fragmentShaderSource = getFileContents(fragmentShaderSourceDir);

	m_ID = createProgram(vertexShaderSource, fragmentShaderSource);

	//findAndAddUniforms(vertexShaderSource);
	//findAndAddUniforms(fragmentShaderSource);
}

void Shader::bind() const
{
	GLAPI(glUseProgram(m_ID));
}

void Shader::unbind() const
{
	GLAPI(glUseProgram(0));
}

Shader::~Shader()
{
	GLAPI(glDeleteProgram(m_ID));
}