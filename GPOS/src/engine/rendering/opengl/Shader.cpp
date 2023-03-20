#include "Shader.h"

#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include "engine/Debug.h"
#include "maths/Maths.h"

#include "maths/Vec3.h"
#include "maths/Vec4.h"

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
	// send 64 bit int as 2 32 bit ints by bit shifting
	GLAPI(glUniform2ui(getUniformLocation(name), static_cast<GLuint>(value), static_cast<GLuint>(value >> 32)));
}

void Shader::setUniform1iv(const std::string& name, size_t count, const int* value)
{
	GLAPI(glUniform1iv(getUniformLocation(name), static_cast<GLsizei>(count), value));
}

unsigned int compileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);

	const char* charSource = source.c_str();
	GLAPI(glShaderSource(id, 1, &charSource, nullptr));

	GLAPI(glCompileShader(id));

	int result;
	GLAPI(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

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
	unsigned int program = glCreateProgram());

	unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	GLAPI(glAttachShader(program, vertexShader));
	GLAPI(glAttachShader(program, fragmentShader));

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

	GLAPI(glDeleteShader(vertexShader));
	GLAPI(glDeleteShader(fragmentShader));

	return program;
}

Shader::Shader(const std::string& vertexShaderSourceDir, const std::string& fragmentShaderSourceDir)
{
	const auto vertexShaderSource = getFileContents(vertexShaderSourceDir);
	const auto fragmentShaderSource = getFileContents(fragmentShaderSourceDir);

	m_ID = createProgram(vertexShaderSource, fragmentShaderSource);
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