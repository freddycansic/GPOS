#pragma once

#include <array>
#include <memory>

#include "Key.h"
#include "MouseButton.h"

inline constexpr int JUST_RELEASED = 3;

// stolen from glfw3.h lines ~315 - ~518
namespace Keys
{
	const std::shared_ptr<Key> SPACE = std::make_shared<Key>(32, "Space");
	const std::shared_ptr<Key> APOSTROPHE = std::make_shared<Key>(39, "'");
	const std::shared_ptr<Key> COMMA = std::make_shared<Key>(44, ",");
	const std::shared_ptr<Key> MINUS = std::make_shared<Key>(45, "-");
	const std::shared_ptr<Key> PERIOD = std::make_shared<Key>(46, ".");
	const std::shared_ptr<Key> SLASH = std::make_shared<Key>(47, "/");
	const std::shared_ptr<Key> ZERO = std::make_shared<Key>(48, "0");
	const std::shared_ptr<Key> ONE = std::make_shared<Key>(49, "1");
	const std::shared_ptr<Key> TWO = std::make_shared<Key>(50, "2");
	const std::shared_ptr<Key> THREE = std::make_shared<Key>(51, "3");
	const std::shared_ptr<Key> FOUR = std::make_shared<Key>(52, "4");
	const std::shared_ptr<Key> FIVE = std::make_shared<Key>(53, "5");
	const std::shared_ptr<Key> SIX = std::make_shared<Key>(54, "6");
	const std::shared_ptr<Key> SEVEN = std::make_shared<Key>(55, "7");
	const std::shared_ptr<Key> EIGHT = std::make_shared<Key>(56, "8");
	const std::shared_ptr<Key> NINE = std::make_shared<Key>(57, "9");
	const std::shared_ptr<Key> SEMICOLON = std::make_shared<Key>(59, ";");
	const std::shared_ptr<Key> EQUAL = std::make_shared<Key>(61, "=");
	const std::shared_ptr<Key> A = std::make_shared<Key>(65, "A");
	const std::shared_ptr<Key> B = std::make_shared<Key>(66, "B");
	const std::shared_ptr<Key> C = std::make_shared<Key>(67, "C");
	const std::shared_ptr<Key> D = std::make_shared<Key>(68, "D");
	const std::shared_ptr<Key> E = std::make_shared<Key>(69, "E");
	const std::shared_ptr<Key> F = std::make_shared<Key>(70, "F");
	const std::shared_ptr<Key> G = std::make_shared<Key>(71, "G");
	const std::shared_ptr<Key> H = std::make_shared<Key>(72, "H");
	const std::shared_ptr<Key> I = std::make_shared<Key>(73, "I");
	const std::shared_ptr<Key> J = std::make_shared<Key>(74, "J");
	const std::shared_ptr<Key> K = std::make_shared<Key>(75, "K");
	const std::shared_ptr<Key> L = std::make_shared<Key>(76, "L");
	const std::shared_ptr<Key> M = std::make_shared<Key>(77, "M");
	const std::shared_ptr<Key> N = std::make_shared<Key>(78, "N");
	const std::shared_ptr<Key> O = std::make_shared<Key>(79, "O");
	const std::shared_ptr<Key> P = std::make_shared<Key>(80, "P");
	const std::shared_ptr<Key> Q = std::make_shared<Key>(81, "Q");
	const std::shared_ptr<Key> R = std::make_shared<Key>(82, "R");
	const std::shared_ptr<Key> S = std::make_shared<Key>(83, "S");
	const std::shared_ptr<Key> T = std::make_shared<Key>(84, "T");
	const std::shared_ptr<Key> U = std::make_shared<Key>(85, "U");
	const std::shared_ptr<Key> V = std::make_shared<Key>(86, "V");
	const std::shared_ptr<Key> W = std::make_shared<Key>(87, "W");
	const std::shared_ptr<Key> X = std::make_shared<Key>(88, "X");
	const std::shared_ptr<Key> Y = std::make_shared<Key>(89, "Y");
	const std::shared_ptr<Key> Z = std::make_shared<Key>(90, "Z");
	const std::shared_ptr<Key> LEFT_BRACKET		= std::make_shared<Key>(91, "(");
	const std::shared_ptr<Key> BACKSLASH		= std::make_shared<Key>(92, "\\");
	const std::shared_ptr<Key> RIGHT_BRACKET	= std::make_shared<Key>(93, ")");
	const std::shared_ptr<Key> GRAVE_ACCENT		= std::make_shared<Key>(96, "`");
	const std::shared_ptr<Key> WORLD_1			= std::make_shared<Key>(161, "World 1");
	const std::shared_ptr<Key> WORLD_2			= std::make_shared<Key>(162, "World 1");
	const std::shared_ptr<Key> ESCAPE			= std::make_shared<Key>(256, "Escape");
	const std::shared_ptr<Key> ENTER			= std::make_shared<Key>(257, "Enter");
	const std::shared_ptr<Key> TAB				= std::make_shared<Key>(258, "Tab");
	const std::shared_ptr<Key> BACKSPACE		= std::make_shared<Key>(259, "Backspace");
	const std::shared_ptr<Key> INSERT			= std::make_shared<Key>(260, "Insert");
	const std::shared_ptr<Key> DEL				= std::make_shared<Key>(261, "Delete"); // cannot name variable delete since it's a keyword
	const std::shared_ptr<Key> RIGHT			= std::make_shared<Key>(262, "Right");
	const std::shared_ptr<Key> LEFT				= std::make_shared<Key>(263, "Left");
	const std::shared_ptr<Key> DOWN				= std::make_shared<Key>(264, "Down");
	const std::shared_ptr<Key> UP				= std::make_shared<Key>(265, "Up");
	const std::shared_ptr<Key> PAGE_UP			= std::make_shared<Key>(266, "Page Up");
	const std::shared_ptr<Key> PAGE_DOWN		= std::make_shared<Key>(267, "Page Down");
	const std::shared_ptr<Key> HOME				= std::make_shared<Key>(268, "Home");
	const std::shared_ptr<Key> END				= std::make_shared<Key>(269, "End");
	const std::shared_ptr<Key> CAPS_LOCK		= std::make_shared<Key>(280, "Caps Lock");
	const std::shared_ptr<Key> SCROLL_LOCK		= std::make_shared<Key>(281, "Scroll Lock");
	const std::shared_ptr<Key> NUM_LOCK			= std::make_shared<Key>(282, "Num Lock");
	const std::shared_ptr<Key> PRINT_SCREEN		= std::make_shared<Key>(283, "Print Screen");
	const std::shared_ptr<Key> PAUSE			= std::make_shared<Key>(284, "Pause");
	const std::shared_ptr<Key> F1 = std::make_shared<Key>(290, "F1");
	const std::shared_ptr<Key> F2 = std::make_shared<Key>(291, "F2");
	const std::shared_ptr<Key> F3 = std::make_shared<Key>(292, "F3");
	const std::shared_ptr<Key> F4 = std::make_shared<Key>(293, "F4");
	const std::shared_ptr<Key> F5 = std::make_shared<Key>(294, "F5");
	const std::shared_ptr<Key> F6 = std::make_shared<Key>(295, "F6");
	const std::shared_ptr<Key> F7 = std::make_shared<Key>(296, "F7");
	const std::shared_ptr<Key> F8 = std::make_shared<Key>(297, "F8");
	const std::shared_ptr<Key> F9 = std::make_shared<Key>(298, "F9");
	const std::shared_ptr<Key> F10 = std::make_shared<Key>(299, "F10");
	const std::shared_ptr<Key> F11 = std::make_shared<Key>(300, "F11");
	const std::shared_ptr<Key> F12 = std::make_shared<Key>(301, "F12");
	const std::shared_ptr<Key> F13 = std::make_shared<Key>(302, "F13");
	const std::shared_ptr<Key> F14 = std::make_shared<Key>(303, "F14");
	const std::shared_ptr<Key> F15 = std::make_shared<Key>(304, "F15");
	const std::shared_ptr<Key> F16 = std::make_shared<Key>(305, "F16");
	const std::shared_ptr<Key> F17 = std::make_shared<Key>(306, "F17");
	const std::shared_ptr<Key> F18 = std::make_shared<Key>(307, "F18");
	const std::shared_ptr<Key> F19 = std::make_shared<Key>(308, "F19");
	const std::shared_ptr<Key> F20 = std::make_shared<Key>(309, "F20");
	const std::shared_ptr<Key> F21 = std::make_shared<Key>(310, "F21");
	const std::shared_ptr<Key> F22 = std::make_shared<Key>(311, "F22");
	const std::shared_ptr<Key> F23 = std::make_shared<Key>(312, "F23");
	const std::shared_ptr<Key> F24 = std::make_shared<Key>(313, "F24");
	const std::shared_ptr<Key> F25 = std::make_shared<Key>(314, "F25");
	const std::shared_ptr<Key> KP_0 = std::make_shared<Key>(320, "Keypad 0");
	const std::shared_ptr<Key> KP_1 = std::make_shared<Key>(321, "Keypad 1");
	const std::shared_ptr<Key> KP_2 = std::make_shared<Key>(322, "Keypad 2");
	const std::shared_ptr<Key> KP_3 = std::make_shared<Key>(323, "Keypad 3");
	const std::shared_ptr<Key> KP_4 = std::make_shared<Key>(324, "Keypad 4");
	const std::shared_ptr<Key> KP_5 = std::make_shared<Key>(325, "Keypad 5");
	const std::shared_ptr<Key> KP_6 = std::make_shared<Key>(326, "Keypad 6");
	const std::shared_ptr<Key> KP_7 = std::make_shared<Key>(327, "Keypad 7");
	const std::shared_ptr<Key> KP_8 = std::make_shared<Key>(328, "Keypad 8");
	const std::shared_ptr<Key> KP_9 = std::make_shared<Key>(329, "Keypad 9");
	const std::shared_ptr<Key> KP_DECIMAL = std::make_shared<Key>(330, "Keypad Decimal");
	const std::shared_ptr<Key> KP_DIVIDE = std::make_shared<Key>(331, "Keypad Divide");
	const std::shared_ptr<Key> KP_MULTIPLY = std::make_shared<Key>(332, "Keypad Multiply");
	const std::shared_ptr<Key> KP_SUBTRACT = std::make_shared<Key>(333, "Keypad Subtract");
	const std::shared_ptr<Key> KP_ADD = std::make_shared<Key>(334, "Keypad Add");
	const std::shared_ptr<Key> KP_ENTER = std::make_shared<Key>(335, "Keypad Enter");
	const std::shared_ptr<Key> KP_EQUAL = std::make_shared<Key>(336, "Keypad Equals");
	const std::shared_ptr<Key> LEFT_SHIFT = std::make_shared<Key>(340, "Shift");
	const std::shared_ptr<Key> LEFT_CONTROL = std::make_shared<Key>(341, "Ctrl");
	const std::shared_ptr<Key> LEFT_ALT = std::make_shared<Key>(342, "Alt");
	const std::shared_ptr<Key> LEFT_SUPER = std::make_shared<Key>(343, "Super");
	const std::shared_ptr<Key> RIGHT_SHIFT = std::make_shared<Key>(344, "Right Shift");
	const std::shared_ptr<Key> RIGHT_CONTROL = std::make_shared<Key>(345, "Right Ctrl");
	const std::shared_ptr<Key> RIGHT_ALT = std::make_shared<Key>(346, "Right Alt");
	const std::shared_ptr<Key> RIGHT_SUPER = std::make_shared<Key>( 347, "Right Super");
	const std::shared_ptr<Key> MENU = std::make_shared<Key>( 348, "Menu");

	inline static std::array<int, 348 + 1> states;
}

namespace MouseButtons
{
	const std::shared_ptr<MouseButton> MOUSE_1 = std::make_shared<MouseButton>(0, "Left Click");
	const std::shared_ptr<MouseButton> MOUSE_2 = std::make_shared<MouseButton>( 1, "Right Click");
	const std::shared_ptr<MouseButton> MOUSE_3 = std::make_shared<MouseButton>( 2, "Middle Click");
	const std::shared_ptr<MouseButton> MOUSE_4 = std::make_shared<MouseButton>( 3, "Mouse 4");
	const std::shared_ptr<MouseButton> MOUSE_5 = std::make_shared<MouseButton>( 4, "Mouse 5");
	const std::shared_ptr<MouseButton> MOUSE_6 = std::make_shared<MouseButton>( 5, "Mouse 6");
	const std::shared_ptr<MouseButton> MOUSE_7 = std::make_shared<MouseButton>( 6, "Mouse 7");
	const std::shared_ptr<MouseButton> MOUSE_8 = std::make_shared<MouseButton>( 7, "Mouse 8");

	inline static std::array<int, 7 + 1> states;
}