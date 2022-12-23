#pragma once

#include <string_view>

struct Button
{
	Button(int buttonCode, const std::string_view& buttonName);
	virtual ~Button() = default;

	[[nodiscard]] virtual bool isDown() const = 0;
	[[nodiscard]] virtual bool isJustReleased() const = 0;

	bool operator==(const Button& other) const;

	int code = 0;
	std::string_view name;
};

template <>
struct std::hash<Button>
{
	std::size_t operator()(const Button& button) const noexcept
	{
		return std::hash<int>()(button.code) ^ std::hash<std::string_view>()(button.name);
	}
};