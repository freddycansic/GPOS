#include "MouseButton.h"

#include "Buttons.h"

#include "GLFW/glfw3.h"

MouseButton::MouseButton(int code, const std::string_view& name) : Button(code, name)
{
}

bool MouseButton::isDown() const
{
	return MouseButtons::states[code] == GLFW_PRESS || MouseButtons::states[code] == GLFW_REPEAT;
}

bool MouseButton::isJustReleased() const
{
	if (MouseButtons::states[code] == JUST_RELEASED)
	{
		MouseButtons::states[code] = GLFW_RELEASE;
		return true;
	}

	return false;
}