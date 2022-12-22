#pragma once

#include <string_view>

#include "Button.h"

struct MouseButton : Button
{
	[[nodiscard]] bool isDown() const override;
	[[nodiscard]] bool isJustReleased() const override;

	MouseButton(int code, const std::string_view& name);
	~MouseButton() override = default;
};