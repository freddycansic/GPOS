#pragma once

#include <string_view>

#include "Button.h"

struct Key final : Button
{
	[[nodiscard]] bool isDown() const override;
	[[nodiscard]] bool isJustReleased() const override;

	Key(int code, const std::string_view& name);
};