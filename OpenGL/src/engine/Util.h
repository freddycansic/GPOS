#pragma once

#include <limits>
#include <vector>

#include "rendering/object/shapes/Cube.h"

struct Vec3;
struct Vec4;

namespace Util
{
	float rgbToHue(const Vec4& rgba);
	float rgbToHue(const Vec3& rgb);

	template <typename T>
	void printVec(const std::vector<T>& vec, const char* delimitor = "\n")
	{
		for (const auto& el : vec)
		{
			std::cout << el << delimitor;
		}
		std::cout << std::endl;
	}

	// https://gist.github.com/alexshtf/eb5128b3e3e143187794
	template <typename T>
	constexpr T sqrtNewtonRaphson(T x, T curr, T prev)
	{
		return curr == prev
			? curr
			: sqrtNewtonRaphson(x, static_cast<T>(0.5) * (curr + x / curr), curr);
	}

	template <typename T>
	constexpr T sqrt(T x) requires std::is_arithmetic_v<T>
	{
		return x >= 0 && x < std::numeric_limits<T>::infinity()
			? sqrtNewtonRaphson(x, x, static_cast<T>(0))
			: std::numeric_limits<T>::quiet_NaN();
	}
}