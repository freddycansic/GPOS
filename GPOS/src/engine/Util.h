#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <ranges>

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

	template <typename T>
	void clearStack(std::stack<T>& stack)
	{
		while (!stack.empty()) stack.pop();
	}

	template <typename T>
	void removeStackIndex(std::stack<T>& stack, size_t index)
	{
		const auto numItemsAfterIndex = stack.size() - index - 1;

		std::vector<T> afterItems;
		afterItems.reserve(numItemsAfterIndex);

		for (size_t i = 0; i < numItemsAfterIndex; ++i)
		{
			afterItems.push_back(stack.top());
			stack.pop();
		}

		stack.pop(); // remove item at index

		for (const auto& item : std::ranges::views::reverse(afterItems))
		{
			stack.push(item);
		}
	}

	void printColoured(const char* text, TextColour colour = TextColour::WHITE);

	std::string replaceAll(std::string original, const std::string& match, const std::string& replace);

	std::string extractFileName(const std::string& path);
}