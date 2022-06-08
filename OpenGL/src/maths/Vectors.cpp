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

Vec4 Vec4::operator*(const Mat4& mat) const {
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