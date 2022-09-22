#pragma once
#include <unordered_map>

struct Key;

namespace Keybinds
{
	std::vector<Key> getKeybindsVec(void(*function)());

	template<typename... Args>
	void setFunctionKeybind(void(*function)(), const Key& key, const Args&... rest)
	{
		getKeybindsVec(function) = { key, rest... };
	}
	
}