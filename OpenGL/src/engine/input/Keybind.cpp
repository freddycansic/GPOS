#include "Keybind.h"

#include "Keys.h"

Keybind::Keybind(const std::initializer_list<Key>& list) :
	m_Keys(list), m_StrRepr(keybindToString(list))
{
}

void Keybind::setKeyBind(const std::vector<Key>& keys)
{
	m_Keys = keys;

	m_StrRepr = keybindToString(keys);
}

std::string Keybind::keybindToString(const std::vector<Key>& keys)
{
	using namespace std::string_literals;

	std::string result;

	for (unsigned int i = 0; i < keys.size()-1; ++i)
	{
		result += std::string(keys.at(i).name) + "+"s;
	}
	result += std::string(keys.at(keys.size() - 1).name);

	return result;
}

std::vector<Key> Keybind::getKeys() const
{
    return m_Keys;
}

std::string Keybind::toString() const
{
    return m_StrRepr;
}

std::ostream& operator<<(std::ostream& os, const Keybind& keybind)
{
	return os << keybind.toString();
}
