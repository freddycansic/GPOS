#include "Maths.h"

#include <random>
#include <chrono>

#include "Vectors.h"

std::mt19937 mersenne(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

namespace Maths {

	float radians(float degrees) {
		return degrees * static_cast<float>(PI_OVER_ONE_EIGHTY);
	}

	float degrees(float radians) {
		return radians * static_cast<float>(ONE_EIGHTY_OVER_PI);
	}

	float randf(float min, float max) {
		std::uniform_real_distribution<float> dist(min, max);
		return dist(mersenne);
	}

	int randint(int min, int max) {
		std::uniform_int_distribution<int> dist(min, max);
		return dist(mersenne);
	}

	Mat4x4 lookAt(const Vec3& position, const Vec3& target, const Vec3& up)
	{
		const Vec3 camDir = (position - target).normalise(); // vector from cam pos to target
		const Vec3 camRight = (camDir.cross(up)).normalise(); // vector pointing in right direction from camera
		const Vec3 camUp = camDir.cross(camRight); // vector pointing in up direction from camera

		return

		Mat4x4{
			{camRight.x, camRight.y, camRight.z, 0},
			{camUp.x, camUp.y, camUp.z, 0},
			{camDir.x, camDir.y, camDir.z, 0},
			{0, 0, 0, 1}
		}

		*

		Mat4x4{
			{1, 0, 0, -position.x},
			{ 0, 1, 0, -position.y },
			{ 0, 0, 1, -position.z },
			{ 0, 0, 0, 1 }
		};

		//return
		//{
		//	{camRight.x, camRight.y, camRight.z, -position.x*camRight.x-camRight.y*position.y-camRight.z*position.z},
		//	{camUp.x, camUp.y, camUp.z, -position.z*camUp.z-camUp.x*position.x-camUp.y*position.x-camUp.y*position.y},
		//	{camDir.x, camDir.y, camDir.z, -position.z*camDir.z-position.x*camDir.x-position.y*camDir.y},
		//	{0, 0, 0, 1}
		//};
	}

	Mat4x4 perspective(float fovRad, float aspect, float near, float far) {
		const float f = 1 / tan(fovRad / 2);

		return {
			{f / aspect, 0, 0, 0},
			{0, f, 0, 0},
			{0, 0, (far + near) / (near - far), (2 * far * near) / (near - far)},
			{0, 0, -1, 0}
		};
	}

	Mat4x4 ortho(float left, float right, float top, float bottom, float near, float far) {
		return {
			{2 / (right - left), 0, 0, -(right + left) / (right - left)},
			{0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom)},
			{0, 0, -2 / (far - near), -(far + near) / (far - near)},
			{0, 0, 0, 1}
		};
	}

	Mat4x4 scale(const Mat4x4& mat, float xScale, float yScale, float zScale)
	{
		return
			mat

			*

			Mat4x4({
				{xScale, 0, 0, 0},
				{0, yScale, 0, 0},
				{0, 0, zScale, 0},
				{ 0, 0, 0, 1 }
			});
	}

	Mat4x4 rotate(const Mat4x4& mat, float xRotate, float yRotate, float zRotate)
	{
		const float x = Maths::radians(xRotate);
		const float y = Maths::radians(yRotate);
		const float z = Maths::radians(zRotate);

		return (
			mat

			*

			Mat4x4({
				{cos(y) * cos(z), -cos(y) * sin(z), sin(y), 0},
				{sin(x) * sin(y) * cos(z) + cos(x) * sin(z), cos(x) * cos(z) - sin(x) * sin(y) * sin(z), -sin(x) * cos(y), 0},
				{sin(x) * sin(z) - cos(x) * sin(y) * cos(z), cos(x) * sin(y) * sin(z) + sin(x) * cos(z), cos(x) * cos(y), 0},
				{0, 0, 0, 1}
			})
			);
	}

	Mat4x4 rotateAxis(const Mat4x4& mat, float xAxis, float yAxis, float zAxis, float theta)
	{
		return (
			mat

			*

			Mat4x4({
				{cos(theta) + xAxis * xAxis * (1 - cos(theta)), xAxis * yAxis * (1 - cos(theta)) - zAxis * sin(theta), xAxis * zAxis * (1 - cos(theta)) + yAxis * sin(theta), 0},
				{yAxis * xAxis * (1 - cos(theta)) + zAxis * sin(theta), cos(theta) + yAxis * yAxis * (1 - cos(theta)), yAxis * zAxis * (1 - cos(theta)) - xAxis * sin(theta), 0},
				{zAxis * xAxis * (1 - cos(theta)) - yAxis * sin(theta), zAxis * yAxis * (1 - cos(theta)) + xAxis * sin(theta), cos(theta) + zAxis * zAxis * (1 - cos(theta)), 0},
				{0, 0, 0, 1}
				}));
	}

	Mat4x4 translate(const Mat4x4& mat, float xTranslate, float yTranslate, float zTranslate)
	{
		return
			mat

			*

			Mat4x4({
				{1, 0, 0, xTranslate},
				{0, 1, 0, yTranslate},
				{0, 0, 1, zTranslate},
				{0, 0, 0, 1}
			});
	}

	Mat4x4 scale(const Mat4x4& mat, const Vec3& scale)
	{
		return Maths::scale(mat, scale.x, scale.y, scale.z);
	}

	Mat4x4 rotate(const Mat4x4& mat, const Vec3& rotation)
	{
		return rotate(mat, rotation.x, rotation.y, rotation.z);
	}

	Mat4x4 rotateAxis(const Mat4x4& mat, const Vec3& axis, float theta)
	{
		return Maths::rotateAxis(mat, axis.x, axis.y, axis.z, theta);
	}

	Mat4x4 translate(const Mat4x4& mat, const Vec3& translation)
	{
		return Maths::translate(mat, translation.x, translation.y, translation.z);
	}
}