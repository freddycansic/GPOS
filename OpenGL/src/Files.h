#pragma once

#include <string>

namespace Files {

	std::string internal(std::string filePath) {
		return "res/" + filePath;
	}

	std::string external(std::string filePath) {
		return filePath;
	}

}
