#include "Files.h"

#include <iostream>

#include "nfd.h"
#include "engine/Debug.h"

namespace Files {

	std::string internal(const std::string& filePath) {
		return "res/" + filePath;
	}

	std::string external(const std::string& filePath) {
		return filePath;
	}

	const char* getPathFromDialogue(const char* filters)
	{
		nfdchar_t* outPath = nullptr;

		if (const auto result = NFD_OpenDialog(filters, nullptr, &outPath); result == NFD_OKAY) 
		{
			return outPath;
		}
		else if (result == NFD_CANCEL)
		{
			return nullptr;
		}

		ASSERT_WITH_MSG(false, NFD_GetError());
	}
}
