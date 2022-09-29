#pragma once

#include <string>

namespace Files {
	// todo make const char*
	std::string internal(const std::string& filePath);
	std::string external(const std::string& filePath);

}
