#pragma once

#include <concepts>
#include <limits>
#include <vector>

#include "imgui/imgui.h"
#include "rendering/object/shapes/Cube.h"

struct Vec3;
struct Vec4;

namespace Util
{
	bool isMouseHoveredWindow(const ImVec2& mousePos, const ImVec2& windowPos, const ImVec2& windowSize);

	Cube getAABBFromPoints(const std::vector<Vec3>& points);

	float rgbToHue(const Vec4& rgba);
	float rgbToHue(const Vec3& rgb);

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