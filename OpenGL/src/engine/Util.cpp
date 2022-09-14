#include "Util.h"

ImVec2 Util::operator+(const ImVec2& self, const ImVec2& other)
{
    return { self.x + other.x, self.y + other.y };
}
