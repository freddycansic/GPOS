#pragma once

#include <GL/glew.h>

#include <string>
#include <iostream>
#include <map>
#include <vector>

#include "maths/Mat4.h"

std::ostream& operator<<(std::ostream& os, const std::map<std::string, int>& map);

class Shader
{
private:
	unsigned int m_ID;
	//std::vector<Uniform> m_Uniforms;
	std::map<std::string, int> m_Uniforms;

	void findAndAddUniforms(const std::string& filepath);

public:
	// get what uniforms the shader has
	Shader(const std::string& vertexShaderSourceDir, const std::string& fragmentShaderSourceDir);
	~Shader();
	
	void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
	void setUniformMat4(const std::string& name, const Mat4& matrix);
	/*
	parse shader source code to find uniforms
	uniform = name, type, location
	
	*/
	//std::vector<uniform>;

	void bind() const;
	void unbind() const;
};

