#pragma once
#include <string>
#include <vector>
#include <ostream>

struct Key;

class Keybind
{
public:
	Keybind(const std::initializer_list<Key>& list);

	void setKeyBind(const std::vector<Key>& keys);

	[[nodiscard]] std::vector<Key> getKeys() const;
	[[nodiscard]] std::string toString() const;

	friend std::ostream& operator<<(std::ostream& os, const Keybind& keybind);

private:
	std::vector<Key> m_Keys;
	std::string m_StrRepr;

	static std::string keybindToString(const std::vector<Key>& keys);
};
