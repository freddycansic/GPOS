#pragma once

#include "imgui/imgui.h"

namespace Util
{
	bool isMouseHoveredWindow(const ImVec2& mousePos, const ImVec2& windowPos, const ImVec2& windowSize);
}
