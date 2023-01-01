#pragma once

#include <string_view>

#include "Button.h"

struct MouseButton : Button
{
	[[nodiscard]] bool isDown() const override;
	[[nodiscard]] bool isJustReleased() const override;

	MouseButton(int mouseCode, const std::string_view& mouseName);
	~MouseButton() override = default;

	static void resetJustReleased();

	static std::array<int, 8 + 1> states;
};