#pragma once

#include <iostream>
#include <concepts>

#include "Mat4.h"
#include "Maths.h"

struct Vec2 {
	float x = 0.0f, y = 0.0f;

	[[nodiscard]] Vec2 normalise() const;
	[[nodiscard]] float magnitude() const;
	[[nodiscard]] float dot(const Vec2& other) const;
	[[nodiscard]] float angleBetween(const Vec2& other) const;
};

struct Vec4; // forward delaration for vec3 conversion

struct Vec3 {
	union { float x, y, z; };
	union { float r, g, b; };

	Vec3() = default;
	Vec3(float x, float y, float z);
	
	// convert vec4 to vec3
	explicit Vec3(const Vec4& vec4);

	Vec3 operator+(const Vec3& other) const;
	Vec3 operator-(const Vec3& other) const;
	void operator+=(const Vec3& other);
	void operator-=(const Vec3& other);

	template <typename T> requires std::convertible_to<T, float>
	Vec3 operator*(const T& scalar) {
		return { x * scalar, y * scalar, z * scalar };
	}

	[[nodiscard]] Vec3 normalise() const;
	[[nodiscard]] float magnitude() const;
	[[nodiscard]] float dot(const Vec3& other) const;
	[[nodiscard]] Vec3 cross(const Vec3& other) const;
	[[nodiscard]] float angleBetween(const Vec3& other) const;
};

struct Vec4 {
	union { float x, y, z, w; };
	union { float r, g, b, a; };

	Vec4() = default;
	Vec4(float x, float y, float z, float w);

	explicit Vec4(const Vec3& vec3, float w);
	explicit Vec4(const Vec3& vec3);

	Vec4 operator*(const Mat4x4& mat) const;

	// TODO move this somewhere else
	[[nodiscard]] Vec4 mixColour(const Vec4& other)
	{
		static constexpr float f = 0.8f; // factor
		return { r * f + other.r * f, g * f + other.g * f, b * f + other.b * f, a * f + other.a * f };
	}
};

std::ostream& operator<<(std::ostream& os, const Vec4& vector);
std::ostream& operator<<(std::ostream& os, const Vec3& vector);
std::ostream& operator<<(std::ostream& os, const Vec2& vector);