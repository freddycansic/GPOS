#pragma once

#include "engine/rendering/object/shapes/Object.h"

struct Ray;
struct Vec3;

namespace Scene
{
	void addObject(std::unique_ptr<Object>&& objectPtr);
	void render();
	void handleMouseClicks();
}
