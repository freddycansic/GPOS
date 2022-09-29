#pragma once

#include "imgui/imgui.h"

struct Vec3;
struct Vec4;

namespace Util
{
	bool isMouseHoveredWindow(const ImVec2& mousePos, const ImVec2& windowPos, const ImVec2& windowSize);

	float rgbToHue(const Vec4& rgba);
	float rgbToHue(const Vec3& rgb);
}
