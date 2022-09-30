#pragma once

#include <string_view>

struct Key
{
	int keyCode;
	std::string_view name;
};

// stolen from glfw3.h lines ~315 - ~518
namespace Keys
{
	constexpr Key SPACE			  =   {32, "Space"};
	constexpr Key APOSTROPHE	  =   {39, "'" };
	constexpr Key COMMA           =   {44, ","};
	constexpr Key MINUS           =   {45, "-"};
	constexpr Key PERIOD          =   {46, "."};
	constexpr Key SLASH           =   {47, "/"};
	constexpr Key ZERO            =   {48, "0"};
	constexpr Key ONE			  =   {49, "1"};
	constexpr Key TWO			  =   {50, "2"};
	constexpr Key THREE           =   {51, "3"};
	constexpr Key FOUR			  =   {52, "4"};
	constexpr Key FIVE			  =   {53, "5"};
	constexpr Key SIX			  =   {54, "6"};
	constexpr Key SEVEN		      =   {55, "7"};
	constexpr Key EIGHT		      =   {56, "8"};
	constexpr Key NINE			  =   {57, "9"};
	constexpr Key SEMICOLON       =   {59, ";"};
	constexpr Key EQUAL           =   {61, "="};
	constexpr Key A               =   {65, "A"};
	constexpr Key B               =   {66, "B"};
	constexpr Key C               =   {67, "C"};
	constexpr Key D               =   {68, "D"};
	constexpr Key E               =   {69, "E"};
	constexpr Key F               =   {70, "F"};
	constexpr Key G               =   {71, "G"};
	constexpr Key H               =   {72, "H"};
	constexpr Key I               =   {73, "I"};
	constexpr Key J               =   {74, "J"};
	constexpr Key K               =   {75, "K"};
	constexpr Key L               =   {76, "L"};
	constexpr Key M               =   {77, "M"};
	constexpr Key N               =   {78, "N"};
	constexpr Key O               =   {79, "O"};
	constexpr Key P               =   {80, "P"};
	constexpr Key Q               =   {81, "Q"};
	constexpr Key R               =   {82, "R"};
	constexpr Key S               =   {83, "S"};
	constexpr Key T               =   {84, "T"};
	constexpr Key U               =   {85, "U"};
	constexpr Key V               =   {86, "V"};
	constexpr Key W               =   {87, "W"};
	constexpr Key X               =   {88, "X"};
	constexpr Key Y               =   {89, "Y"};
	constexpr Key Z               =   {90, "Z"};
	constexpr Key LEFT_BRACKET    =   {91, "("};  
	constexpr Key BACKSLASH       =   {92, "\\"};  
	constexpr Key RIGHT_BRACKET   =   {93, ")"};  
	constexpr Key GRAVE_ACCENT    =   {96, "`"};  
	constexpr Key WORLD_1		  =   {161, "World 1"};
	constexpr Key WORLD_2		  =   {162, "World 1"};
	constexpr Key ESCAPE		  =   {256, "Escape"};
	constexpr Key ENTER           =   {257, "Enter"};
	constexpr Key TAB             =   {258, "Tab"};
	constexpr Key BACKSPACE       =   {259, "Backspace"};
	constexpr Key INSERT          =   {260, "Insert"};
	constexpr Key DEL			  =   {261, "Delete"}; // cannot name variable delete since it's a keyword
	constexpr Key RIGHT           =   {262, "Right"};
	constexpr Key LEFT            =   {263, "Left"};
	constexpr Key DOWN            =   {264, "Down"};
	constexpr Key UP              =   {265, "Up"};
	constexpr Key PAGE_UP         =   {266, "Page Up"};
	constexpr Key PAGE_DOWN       =   {267, "Page Down"};
	constexpr Key HOME            =   {268, "Home"};
	constexpr Key END             =   {269, "End"};
	constexpr Key CAPS_LOCK       =   {280, "Caps Lock"};
	constexpr Key SCROLL_LOCK     =   {281, "Scroll Lock"};
	constexpr Key NUM_LOCK        =   {282, "Num Lock"};
	constexpr Key PRINT_SCREEN    =   {283, "Print Screen"};
	constexpr Key PAUSE           =   {284, "Pause"};
	constexpr Key F1              =   {290, "F1"};
	constexpr Key F2              =   {291, "F2"};
	constexpr Key F3              =   {292, "F3"};
	constexpr Key F4              =   {293, "F4"};
	constexpr Key F5              =   {294, "F5"};
	constexpr Key F6              =   {295, "F6"};
	constexpr Key F7              =   {296, "F7"};
	constexpr Key F8              =   {297, "F8"};
	constexpr Key F9              =   {298, "F9"};
	constexpr Key F10             =   {299, "F10"};
	constexpr Key F11             =   {300, "F11"};
	constexpr Key F12             =   {301, "F12"};
	constexpr Key F13             =   {302, "F13"};
	constexpr Key F14             =   {303, "F14"};
	constexpr Key F15             =   {304, "F15"};
	constexpr Key F16             =   {305, "F16"};
	constexpr Key F17             =   {306, "F17"};
	constexpr Key F18             =   {307, "F18"};
	constexpr Key F19             =   {308, "F19"};
	constexpr Key F20             =   {309, "F20"};
	constexpr Key F21             =   {310, "F21"};
	constexpr Key F22             =   {311, "F22"};
	constexpr Key F23             =   {312, "F23"};
	constexpr Key F24             =   {313, "F24"};
	constexpr Key F25             =   {314, "F25"};
	constexpr Key KP_0            =   {320, "Keypad 0"};
	constexpr Key KP_1            =   {321, "Keypad 1"};
	constexpr Key KP_2            =   {322, "Keypad 2"};
	constexpr Key KP_3            =   {323, "Keypad 3"};
	constexpr Key KP_4            =   {324, "Keypad 4"};
	constexpr Key KP_5            =   {325, "Keypad 5"};
	constexpr Key KP_6            =   {326, "Keypad 6"};
	constexpr Key KP_7            =   {327, "Keypad 7"};
	constexpr Key KP_8            =   {328, "Keypad 8"};
	constexpr Key KP_9            =   {329, "Keypad 9"};
	constexpr Key KP_DECIMAL      =   {330, "Keypad Decimal"};
	constexpr Key KP_DIVIDE       =   {331, "Keypad Divide"};
	constexpr Key KP_MULTIPLY     =   {332, "Keypad Multiply"};
	constexpr Key KP_SUBTRACT     =   {333, "Keypad Subtract"};
	constexpr Key KP_ADD          =   {334, "Keypad Add"};
	constexpr Key KP_ENTER        =   {335, "Keypad Enter"};
	constexpr Key KP_EQUAL        =   {336, "Keypad Equals"};
	constexpr Key LEFT_SHIFT      =   {340, "Shift"};
	constexpr Key LEFT_CONTROL    =   {341, "Ctrl"};
	constexpr Key LEFT_ALT        =   {342, "Alt"};
	constexpr Key LEFT_SUPER      =   {343, "Super"};
	constexpr Key RIGHT_SHIFT     =   {344, "Right Shift"};
	constexpr Key RIGHT_CONTROL   =   {345, "Right Ctrl"};
	constexpr Key RIGHT_ALT       =   {346, "Right Alt"};
	constexpr Key RIGHT_SUPER     =   {347, "Right Super"};
	constexpr Key MENU            =	  {348, "Menu"};
	constexpr Key LAST            =   MENU;
}

namespace MouseButtons
{
	constexpr Key MOUSE_1         =   {0, "Left Click"};
	constexpr Key MOUSE_2         =   {1, "Right Click"};
	constexpr Key MOUSE_3         =   {2, "Middle Click"};
	constexpr Key MOUSE_4         =   {3, "Mouse 4"};
	constexpr Key MOUSE_5         =   {4, "Mouse 5"};
	constexpr Key MOUSE_6         =   {5, "Mouse 6"};
	constexpr Key MOUSE_7         =   {6, "Mouse 7"};
	constexpr Key MOUSE_8         =   {7, "Mouse 8"};
	constexpr Key LAST			  =   MOUSE_8;
	constexpr Key LEFT      =   MOUSE_1;
	constexpr Key RIGHT     =   MOUSE_2;
	constexpr Key MIDDLE    =   MOUSE_3;
}