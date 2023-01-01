#include "Util.h"

#ifdef _WIN32
	#include <Windows.h>
#endif

namespace Util
{
	void printColoured(const char* text, TextColour colour)
	{
#ifdef _WIN32
		static const auto& COLOURED_TEXT_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);

		switch (colour)
		{
		case TextColour::WHITE:
			break;
		case TextColour::RED:
			SetConsoleTextAttribute(COLOURED_TEXT_HANDLE, FOREGROUND_RED);
			break;
		case TextColour::ORANGE:
			SetConsoleTextAttribute(COLOURED_TEXT_HANDLE, FOREGROUND_RED | FOREGROUND_GREEN);
			break;
		case TextColour::YELLOW:
			SetConsoleTextAttribute(COLOURED_TEXT_HANDLE, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			break;
		case TextColour::LIGHT_BLUE:
			SetConsoleTextAttribute(COLOURED_TEXT_HANDLE, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			break;
		}
#endif

		std::cout << text; // if not on windows just print normally

#ifdef _WIN32
		SetConsoleTextAttribute(COLOURED_TEXT_HANDLE, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
	}

	std::string replaceAll(std::string original, const std::string& match, const std::string& replace)
	{
		size_t pos = 0;
		while ((pos = original.find(match, pos)) != std::string::npos)
		{
			original.replace(pos, match.length(), replace);
			pos += replace.length();
		}

		return original;
	}
}