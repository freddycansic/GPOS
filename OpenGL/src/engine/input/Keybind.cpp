#include "Keybind.h"

#include <set>

#include "Input.h"
#include "Button.h"

std::unique_ptr<std::unordered_map<Keybind, std::string>> Keybind::sp_StringRepresentations = std::make_unique<std::unordered_map<Keybind, std::string>>();

Keybind::Keybind(const std::initializer_list<std::shared_ptr<Button>>& list) :
	m_Buttons(list)
{
}

void Keybind::setKeybind(const std::vector<std::shared_ptr<Button>>& buttons)
{
	m_Buttons = buttons;
}

bool Keybind::isJustReleased() const
{
	unsigned int numKeysDown = 0, numKeysJustReleased = 0;
	const auto& buttons = this->getButtons();

	for (const auto& button : buttons)
	{
		if (button->isDown())
		{
			++numKeysDown;
			continue;
		}

		if (button->isJustReleased())
		{
			++numKeysJustReleased;
		}
	}

	if (numKeysDown >= buttons.size()) return false;

	return numKeysDown + numKeysJustReleased == buttons.size();
}

bool Keybind::isHeld() const
{
	for (const auto& key : this->getButtons())
	{
		if (!key->isDown()) return false;
	}

	return true;
}

const std::vector<std::shared_ptr<Button>>& Keybind::getButtons() const
{
    return m_Buttons;
}

std::string Keybind::toString() const
{
	if (sp_StringRepresentations->contains(*this))
	{
		return sp_StringRepresentations->at(*this);
	}

	using namespace std::string_literals;
		
	std::string result;

	for (unsigned int i = 0; i < m_Buttons.size() - 1; ++i)
	{
		result += std::string(m_Buttons.at(i)->name) + "+"s;
	}
	result += std::string(m_Buttons.at(m_Buttons.size() - 1)->name);

	// i love c++
	sp_StringRepresentations->operator[](*this) = result;

	return result;
}

bool Keybind::operator==(const Keybind& other) const
{
	std::set<std::shared_ptr<Button>> buttons;
	buttons.insert(m_Buttons.begin(), m_Buttons.end());

	std::set< std::shared_ptr<Button>> otherButtons;
	otherButtons.insert(other.getButtons().begin(), other.getButtons().end());

	return buttons == otherButtons;
}

std::ostream& operator<<(std::ostream& os, const Keybind& keybind)
{
	return os << keybind.toString();
}
