#pragma once

#include <iostream>
#include <concepts>

#include <GLEW/glew.h>

#include "Maths.h"

struct Vec4; // forward delaration for vec3 + vec4 conversion

struct Vec2 {
	GLfloat x = 0.0f, y = 0.0f;

	Vec2() = default;
	Vec2(GLfloat x, GLfloat y);

	explicit Vec2(const Vec4& vec4);

	Vec2 operator-(Vec2 other) const;

	[[nodiscard]] Vec2 normalise() const;
	[[nodiscard]] GLfloat magnitude() const;
	[[nodiscard]] GLfloat dot(Vec2 other) const;
	[[nodiscard]] GLfloat angleBetween(Vec2 other) const;
};


struct Vec3 {
	union { GLfloat x = 0.0f, r; };
	union { GLfloat y = 0.0f, g; };
	union { GLfloat z = 0.0f, b; };

	constexpr Vec3() = default;
	constexpr Vec3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {} // only works for constexpr if defined in header
	
	// convert vec4 to vec3
	explicit Vec3(const Vec4& vec4);

	Vec3 operator+(const Vec3& other) const;
	Vec3 operator-(const Vec3& other) const;
	void operator+=(const Vec3& other);
	void operator-=(const Vec3& other);
	void operator/=(GLfloat divisor);

	template <typename T> requires std::convertible_to<T, GLfloat>
	Vec3 operator*(T scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}

	[[nodiscard]] Vec3 normalise() const;
	[[nodiscard]] GLfloat magnitude() const;
	[[nodiscard]] GLfloat dot(const Vec3& other) const;
	[[nodiscard]] Vec3 cross(const Vec3& other) const;
	[[nodiscard]] GLfloat angleBetween(const Vec3& other) const;
};

struct Vec4 {
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
std::ostream& operator<<(std::ostream& os, const Vec3& vector);
std::ostream& operator<<(std::ostream& os, const Vec2& vector);