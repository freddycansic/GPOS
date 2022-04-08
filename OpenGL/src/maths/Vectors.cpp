#include <iostream>

#include "maths/Vectors.h"

Vec3::Vec3(float x, float y, float z) :
	x(x), y(y), z(z)
{
}

Vec3::Vec3(const Vec4& vec4) :
	x(vec4.x), y(vec4.y), z(vec4.z)
{
}

Vec4::Vec4(float x, float y, float z, float w) :
	x(x), y(y), z(z), w(w)
{
}

Vec4::Vec4(const Vec3& vec3, float w) :
	x(vec3.x), y(vec3.y), z(vec3.z), w(w)
{
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