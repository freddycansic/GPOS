#pragma once

#include <iostream>

#include <GL/glew.h>

struct Vec4;

struct Vec2
{
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

std::ostream& operator<<(std::ostream& os, const Vec2& vector);