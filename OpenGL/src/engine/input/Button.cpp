#include "Button.h"

Button::Button(int code, const std::string_view& name) :
	code(code), name(name)
{
}

bool Button::operator==(const Button& other) const
{
	return code == other.code;
}
