#pragma once
#include "Matrix.h"

struct Vec3;

using Mat4x4 = Mat<4, 4, float>;

namespace Maths {

	constexpr double PI = 3.14159265358979323846;
	constexpr double PI_OVER_ONE_EIGHTY = PI / 180;
	constexpr double ONE_EIGHTY_OVER_PI = 180 / PI;

	float radians(float degrees);
	float degrees(float radians);

	float randf(float min, float max);
	int randint(int min, int max);

	Mat4x4 lookAt(const Vec3& position, const Vec3& target, const Vec3& up);
	Mat4x4 perspective(float fovDeg, float aspect, float near, float far);
	Mat4x4 ortho(float left, float right, float top, float bottom, float near, float far);
	Mat4x4 scale(const Mat4x4& mat, float xScale, float yScale, float zScale);
	Mat4x4 rotate(const Mat4x4& mat, float xRotate, float yRotate, float zRotate);
	Mat4x4 rotateAxis(const Mat4x4& mat, float xAxis, float yAxis, float zAxis, float theta);
	Mat4x4 translate(const Mat4x4& mat, float xTranslate, float yTranslate, float zTranslate);
}
