#pragma once

#include <string_view>
#include <array>

#include "Button.h"

struct Key final : Button
{
	[[nodiscard]] bool isDown() const override;
	[[nodiscard]] bool isJustReleased() const override;

	Key(int keyCode, const std::string_view& keyName);

	static void resetJustReleased();

	static std::array<int, 348 + 1> states;
};