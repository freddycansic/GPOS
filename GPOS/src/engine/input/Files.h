#pragma once

#include <string>

namespace Files
{
	// todo make const char*
	std::string internal(const std::string& filePath);
	std::string external(const std::string& filePath);

	const char* getPathFromDialogue(const char* filters = nullptr);
	const char* getSavePathFromDialogue(const char* filters = nullptr);
}
