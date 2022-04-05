#pragma once

#include <GL/glew.h>

#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "maths/Mat4.h"

std::ostream& operator<<(std::ostream& os, const std::unordered_map<std::string, int>& map);

class Shader
{
private:
	unsigned int m_ID;
	std::unordered_map<std::string, int> m_Uniforms;

	void findAndAddUniforms(const std::string& filepath);

public:
	Shader(const std::string& vertexShaderSourceDir, const std::string& fragmentShaderSourceDir);
	~Shader();
	
	void setUniform1i(const std::string& name, int value);
	void setUniform1iv(const std::string& name, unsigned int count, const int* value);
	void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
	void setUniformMat4(const std::string& name, const Mat4& matrix);

	void bind() const;
	void unbind() const;
};

