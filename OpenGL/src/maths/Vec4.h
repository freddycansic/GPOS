#pragma once

#include <iostream>

#include <GL/glew.h>

class Mat4x4;

struct Vec3;

struct Vec4
{
	union { GLfloat x = 0.0f, r; };
	union { GLfloat y = 0.0f, g; };
	union { GLfloat z = 0.0f, b; };
	union { GLfloat w = 0.0f, a; };

	Vec4() = default;
	Vec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

	explicit Vec4(const Vec3& vec3, GLfloat w);
	explicit Vec4(const Vec3& vec3);

	Vec4 operator*(const Mat4x4& mat) const;
	Vec4 operator*(GLfloat multiplier) const;
};

std::ostream& operator<<(std::ostream& os, const Vec4& vector);