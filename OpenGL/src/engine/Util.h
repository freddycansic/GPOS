#pragma once

#include <iostream>
#include <vector>

struct Vec3;
struct Vec4;

enum class TextColour
{
	WHITE,
	RED,
	ORANGE,
	YELLOW,
	LIGHT_BLUE
};

namespace Util
{
	template <typename T>
	concept arithmetic = std::is_arithmetic_v<T>;

	template <typename T>
	void printVec(const std::vector<T>& vec, const char* delimitor = "\n")
	{
		for (const auto& el : vec)
		{
			std::cout << el << delimitor;
		}
		std::cout << std::endl;
	}

	void printColoured(const char* text, TextColour colour = TextColour::WHITE);
}