#include "Util.h"

namespace Util
{
	bool isMouseHoveredWindow(const ImVec2& mousePos, const ImVec2& windowPos, const ImVec2& windowSize)
	{
		return (mousePos.x > windowPos.x && mousePos.x < windowPos.x + windowSize.x && mousePos.y > windowPos.y && mousePos.y < windowPos.y + windowSize.y);
	}
}


