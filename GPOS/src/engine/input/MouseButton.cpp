#include "MouseButton.h"

#include "Buttons.h"

#include "GLFW/glfw3.h"

std::array<int, 8 + 1> MouseButton::states;

MouseButton::MouseButton(int mouseCode, const std::string_view& mouseName) : Button(mouseCode, mouseName)
{
}

bool MouseButton::isDown() const
{
	return states[code] == GLFW_PRESS || states[code] == GLFW_REPEAT;
}

bool MouseButton::isJustReleased() const
{
	if (states[code] == JUST_RELEASED)
	{
		return true;
	}

	return false;
}

void MouseButton::resetJustReleased()
{
	for (auto& state : states)
	{
		if (state == JUST_RELEASED) state = GLFW_RELEASE;
	}
}