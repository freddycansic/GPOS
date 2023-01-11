#include "Button.h"

Button::Button(int buttonCode, const std::string_view& buttonName) :
	code(buttonCode), name(buttonName)
{
}

bool Button::operator==(const Button& other) const
{
	return code == other.code;
}
