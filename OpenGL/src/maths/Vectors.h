#pragma once

#include <iostream>
#include "Mat4.h"

struct Vec2 {
	float x = 0.0f, y = 0.0f;
};

struct Vec4; // forward delaration for vec3 conversion

struct Vec3 {
	float x = 0.0f, y = 0.0f, z = 0.0f;

	Vec3() = default;

	// regular constructor
	Vec3(float x, float y, float z);
	
	// convert vec4 to vec3
	explicit Vec3(const Vec4& vec4);

	Vec3 operator+(const Vec3& other) const;
	Vec3 operator-(const Vec3& other) const;
	[[nodiscard]] Vec3 normalise() const;
	[[nodiscard]] float magnitude() const;
	[[nodiscard]] float dot(const Vec3& other) const;
	[[nodiscard]] Vec3 cross(const Vec3& other) const;
	[[nodiscard]] float angleBetween(const Vec3& other) const;
};

struct Vec4 {
	float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

	Vec4() = default;
	Vec4(float x, float y, float z, float w);
	explicit Vec4(const Vec3& vec3, float w);

	Vec4 operator*(const Mat4& mat) const;
};

std::ostream& operator<<(std::ostream& os, const Vec4& vector);
std::ostream& operator<<(std::ostream& os, const Vec3& vector);