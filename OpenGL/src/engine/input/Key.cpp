#include "Key.h"

#include <iostream>

#include "Buttons.h"

#include "GLFW/glfw3.h"

std::array<int, 348 + 1> Key::states;

Key::Key(int keyCode, const std::string_view& keyName) : Button(keyCode, keyName)
{
}

bool Key::isDown() const
{
	return states[code] == GLFW_PRESS || states[code] == GLFW_REPEAT;
}

bool Key::isJustReleased() const
{
	if (states[code] == JUST_RELEASED)
	{
		states[code] = GLFW_RELEASE;
		return true;
	}

	return false;
}
