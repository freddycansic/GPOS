#include "Vec2.h"

#include "Vec4.h"

Vec2::Vec2(GLfloat x, GLfloat y)
	: x(x), y(y)
{
}

Vec2::Vec2(const Vec4& vec4)
	: x(vec4.x), y(vec4.y)
{
}

Vec2 Vec2::operator-(Vec2 other) const
{
	return { x - other.x, y - other.y };
}

Vec2 Vec2::normalise() const
{
	const auto mag = magnitude();
	return { x / mag, y / mag };
}

GLfloat Vec2::magnitude() const
{
	return Util::sqrt(x * x + y * y);
}

GLfloat Vec2::dot(Vec2 other) const
{
	return x * other.x + y * other.y;
}

GLfloat Vec2::angleBetween(Vec2 other) const
{
	return acos(this->dot(other) / (this->magnitude() * other.magnitude()));
}

std::ostream& operator<<(std::ostream& os, const Vec2& vector) {
	return os << "| " << vector.x << " |\n"
		<< "| " << vector.y << " |\n";
}