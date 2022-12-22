#include "Key.h"

#include "Buttons.h"

#include "GLFW/glfw3.h"

Key::Key(int code, const std::string_view& name) : Button(code, name)
{
}

bool Key::isDown() const
{
	return Keys::states[code] == GLFW_PRESS || Keys::states[code] == GLFW_REPEAT;
}

bool Key::isJustReleased() const
{
	if (Keys::states[code] == JUST_RELEASED)
	{
		Keys::states[code] = GLFW_RELEASE;
		return true;
	}

	return false;
}
