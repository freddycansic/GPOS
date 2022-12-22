#pragma once

#include <string>
#include <vector>
#include <ostream>
#include <unordered_map>
#include <memory>

#include "Button.h"

struct Button;

class Keybind
{
public:
	Keybind(const std::initializer_list<std::shared_ptr<Button>>& list);

	void setKeybind(const std::vector<std::shared_ptr<Button>>& buttons);

	[[nodiscard]] const std::vector<std::shared_ptr<Button>>& getButtons() const;
	[[nodiscard]] std::string toString() const;

	[[nodiscard]] bool isJustReleased() const;
	[[nodiscard]] bool isHeld() const;

	bool operator==(const Keybind& other) const;

 	friend std::ostream& operator<<(std::ostream& os, const Keybind& keybind);

private:
	std::vector<std::shared_ptr<Button>> m_Buttons;

	// unique ptr hack to get around making toString() non const
	static std::unique_ptr<std::unordered_map<Keybind, std::string>> sp_StringRepresentations;
};

template <>
struct std::hash<Keybind>
{
	std::size_t operator()(const Keybind& keybind) const noexcept
	{
		if (keybind.getButtons().empty()) return std::hash<int>()(0);

		size_t result = std::hash<Button>()(*keybind.getButtons()[0]);

		for (size_t i = 1; i < keybind.getButtons().size(); ++i)
		{
			result ^= std::hash<Button>()(*keybind.getButtons()[i]);
		}

		return result;
	}
};