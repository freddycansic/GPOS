#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>
#include <iostream>

struct Uniform {
	std::string name;
	int location;
};

std::ostream& operator<<(std::ostream& os, const Uniform& uniform);

class Shader
{
private:
	unsigned int m_ID;
	std::vector<Uniform> m_Uniforms;

	void findAndAddUniforms(const std::string& filepath);

public:
	// get what uniforms the shader has
	Shader(const std::string& vertexShaderSourceDir, const std::string& fragmentShaderSourceDir);
	~Shader();
	
	/*
	parse shader source code to find uniforms
	uniform = name, type, location
	
	*/
	//std::vector<uniform>;

	void bind() const;
	void unbind() const;
};

