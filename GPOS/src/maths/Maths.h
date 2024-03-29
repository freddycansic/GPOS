#pragma once

#include <concepts>

#include <GL/glew.h>

#include "Matrix.h"
#include "engine/Util.h"

struct Vec3;

using Mat4x4 = Mat<4, 4, GLfloat>;

namespace Maths
{
	template<std::convertible_to<double> T>
	constexpr T PI = static_cast<T>(3.14159265358979323846);

	constexpr double PI_OVER_ONE_EIGHTY = PI<double> / 180;
	constexpr double ONE_EIGHTY_OVER_PI = 180 / PI<double>;

	float radians(float degrees);
	float degrees(float radians);

	float randf(float min, float max);
	int randint(int min, int max);

	Mat4x4 lookAt(const Vec3& position, const Vec3& target, const Vec3& up);
	Mat4x4 perspective(float fovRad, float aspect, float near, float far);

	Mat4x4 scale(const Mat4x4& mat, float xScale, float yScale, float zScale);
	Mat4x4 rotate(const Mat4x4& mat, float xRotate, float yRotate, float zRotate);
	Mat4x4 rotateAxis(const Mat4x4& mat, float xAxis, float yAxis, float zAxis, float theta);
	Mat4x4 translate(const Mat4x4& mat, float xTranslate, float yTranslate, float zTranslate);

	Mat4x4 scale(const Mat4x4& mat, const Vec3& scale);
	Mat4x4 rotate(const Mat4x4& mat, const Vec3& rotation);
	Mat4x4 rotateAxis(const Mat4x4& mat, const Vec3& axis, float theta);
	Mat4x4 translate(const Mat4x4& mat, const Vec3& translation);

	// https://gist.github.com/alexshtf/eb5128b3e3e143187794
	template <Util::arithmetic T>
	constexpr T sqrtNewtonRaphson(T x, T curr, T prev)
	{
		return curr == prev
			? curr
			: sqrtNewtonRaphson(x, static_cast<T>(0.5) * (curr + x / curr), curr);
	}

	template <Util::arithmetic T>
	constexpr T sqrt(T x)
	{
		return x >= 0 && x < std::numeric_limits<T>::infinity()
			? sqrtNewtonRaphson(x, x, static_cast<T>(0))
			: std::numeric_limits<T>::quiet_NaN();
	}
}
