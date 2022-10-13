#include <iostream>

#include "maths/Vectors.h"
#include "engine/Util.h"

Vec2::Vec2(float x, float y)
	: x(x), y(y)
{
}

Vec2 Vec2::normalise() const
{
	const auto mag = magnitude();
	return { x / mag, y / mag };
}

float Vec2::magnitude() const
{
	return Util::sqrt(x * x + y * y);
}

float Vec2::dot(Vec2 other) const
{
	return x * other.x + y * other.y;
}

float Vec2::angleBetween(Vec2 other) const
{
	return acos(this->dot(other) / (this->magnitude() * other.magnitude()));
}

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

void Vec3::operator/=(float divisor)
{
	x /= divisor;
	y /= divisor;
	z /= divisor;
}

float Vec3::magnitude() const
{
	return Util::sqrt(x * x + y * y + z * z);
}

float Vec3::dot(const Vec3& other) const
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

float Vec3::angleBetween(const Vec3& other) const
{
	return acos(this->dot(other) / (this->magnitude() * other.magnitude()));
}

Vec3 Vec3::normalise() const
{
	const auto mag = magnitude();
	return { x / mag, y / mag, z / mag };
}

Vec4::Vec4(float x, float y, float z, float w) :
	x(x), y(y), z(z), w(w)
{
}

Vec4::Vec4(const Vec3& vec3, float w) :
	x(vec3.x), y(vec3.y), z(vec3.z), w(w)
{
}

Vec4::Vec4(const Vec3& vec3) :
	x(vec3.x), y(vec3.y), z(vec3.z), w(1.0f)
{
}

Vec4 Vec4::operator*(float multiplier) const
{
	return { x * multiplier, y * multiplier, z * multiplier, w * multiplier };
}

Vec4 Vec4::operator*(const Mat4x4& mat) const {
	return {
		mat[0][0] * x + mat[0][1] * y + mat[0][2] * z + mat[0][3] * w,
		mat[1][0] * x + mat[1][1] * y + mat[1][2] * z + mat[1][3] * w,
		mat[2][0] * x + mat[2][1] * y + mat[2][2] * z + mat[2][3] * w,
		mat[3][0] * x + mat[3][1] * y + mat[3][2] * z + mat[3][3] * w
	};
}

std::ostream& operator<<(std::ostream& os, const Vec4& vector) {
	return os << "| " << vector.x << " |\n"
		<< "| " << vector.y << " |\n"
		<< "| " << vector.z << " |\n"
		<< "| " << vector.w << " |\n";
}

std::ostream& operator<<(std::ostream& os, const Vec3& vector) {
	return os << "| " << vector.x << " |\n"
		<< "| " << vector.y << " |\n"
		<< "| " << vector.z << " |\n";
}

std::ostream& operator<<(std::ostream& os, const Vec2& vector) {
	return os << "| " << vector.x << " |\n"
		<< "| " << vector.y << " |\n";
}