#pragma once

#include <limits>
#include <vector>

#include "rendering/object/shapes/Cube.h"

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

	// https://gist.github.com/alexshtf/eb5128b3e3e143187794
	template <typename T>
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

#ifdef _WIN32
	#include <Windows.h>
#endif

	template <typename T>
	void printColoured(const T& value, TextColour colour = TextColour::WHITE)
	{
#ifdef _WIN32
		static const auto& COLOURED_TEXT_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);

		switch (colour)
		{
		case TextColour::WHITE:
			break;
		case TextColour::RED:
			SetConsoleTextAttribute(COLOURED_TEXT_HANDLE, FOREGROUND_RED);
			break;
		case TextColour::ORANGE:
			SetConsoleTextAttribute(COLOURED_TEXT_HANDLE, FOREGROUND_RED | FOREGROUND_GREEN);
			break;
		case TextColour::YELLOW:
			SetConsoleTextAttribute(COLOURED_TEXT_HANDLE, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case TextColour::LIGHT_BLUE:
			SetConsoleTextAttribute(COLOURED_TEXT_HANDLE, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		}
#endif

		std::cout << value; // if not on windows just print normally

#ifdef _WIN32
		SetConsoleTextAttribute(COLOURED_TEXT_HANDLE, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
	}
}