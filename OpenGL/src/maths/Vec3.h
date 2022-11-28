#pragma once

#include <iostream>

#include <GL/glew.h>

struct Vec4;

struct Vec3
{
	union { GLfloat x = 0.0f, r; };
	union { GLfloat y = 0.0f, g; };
	union { GLfloat z = 0.0f, b; };

	constexpr Vec3() = default;
	constexpr Vec3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {} // only works for constexpr if defined in header

	explicit Vec3(const Vec4& vec4);

	Vec3 operator+(const Vec3& other) const;
	Vec3 operator-(const Vec3& other) const;
	void operator+=(const Vec3& other);
	void operator-=(const Vec3& other);
	void operator/=(GLfloat divisor);

	Vec3 operator*(GLfloat scalar) const;

	[[nodiscard]] Vec3 normalise() const;
	[[nodiscard]] GLfloat magnitude() const;
	[[nodiscard]] GLfloat dot(const Vec3& other) const;
	[[nodiscard]] Vec3 cross(const Vec3& other) const;
	[[nodiscard]] GLfloat angleBetween(const Vec3& other) const;
};

std::ostream& operator<<(std::ostream& os, const Vec3& vector);