#include "Files.h"

namespace Files {

	std::string internal(const std::string& filePath) {
		return "res/" + filePath;
	}

	std::string external(const std::string& filePath) {
		return filePath;
	}

}