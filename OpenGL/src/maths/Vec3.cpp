#include "Vec3.h"

#include "Maths.h"
#include "Vec4.h"

Vec3::Vec3(const Vec4& vec4) :
	x(vec4.x), y(vec4.y), z(vec4.z)
{
}

Vec3 Vec3::operator+(const Vec3& other) const
{
	return { x + other.x, y + other.y, z + other.z };
}

Vec3 Vec3::operator-(const Vec3& other) const
{
	return { x - other.x, y - other.y, z - other.z };
}

void Vec3::operator+=(const Vec3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
}

void Vec3::operator-=(const Vec3& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

void Vec3::operator/=(GLfloat divisor)
{
	x /= divisor;
	y /= divisor;
	z /= divisor;
}

Vec3 Vec3::operator*(GLfloat scalar) const
{
	return {x * scalar, y * scalar, z * scalar};
}

GLfloat Vec3::magnitude() const
{
	return Maths::sqrt(x * x + y * y + z * z);
}

GLfloat Vec3::dot(const Vec3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::cross(const Vec3& other) const
{
	return {
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x
	};
}

GLfloat Vec3::angleBetween(const Vec3& other) const
{
	return acos(this->dot(other) / (this->magnitude() * other.magnitude()));
}

Vec3 Vec3::normalise() const
{
	const auto mag = magnitude();
	return { x / mag, y / mag, z / mag };
}

std::ostream& operator<<(std::ostream& os, const Vec3& vector)
{
	return os << "| " << vector.x << " |\n"
		<< "| " << vector.y << " |\n"
		<< "| " << vector.z << " |\n";
}
