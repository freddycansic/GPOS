#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

#include "maths/Maths.h"

struct Vec4;

std::ostream& operator<<(std::ostream& os, const std::unordered_map<std::string, int>& map);

class Shader
{
private:
	unsigned int m_ID;
	std::unordered_map<std::string, int> m_Uniforms;

	void findAndAddUniforms(const std::string& filepath);
	void checkUniformInShader(const std::string& name) const;

public:
	Shader(const std::string& vertexShaderSourceDir, const std::string& fragmentShaderSourceDir);
	~Shader();
	
	Shader(const Shader& tex) = delete;
	Shader& operator=(const Shader& tex) = delete;

	void setUniform1i(const std::string& name, int value) const;
	void setUniform1ui64(const std::string& name, uint64_t value) const;
	void setUniform1iv(const std::string& name, size_t count, const int* value) const;
	void setUniform3f(const std::string& name, float v1, float v2, float v3) const;
	void setUniform3f(const std::string& name, const Vec3& vec) const;
	void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4) const;
	void setUniform4f(const std::string& name, const Vec4& vec) const;
	void setUniformMat4(const std::string& name, const Mat4x4& matrix) const;

	void bind() const;
	void unbind() const;
};

