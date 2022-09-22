#include <unordered_map>

#include "Keys.h"
#include "Keybinds.h"

std::unordered_map<void(*)(), std::vector<Key>> keybinds;

namespace Keybinds
{
	std::vector<Key> getKeybindsVec(void(*function)())
	{
		return keybinds[function];
	}
}