#pragma once

#include "engine/rendering/gui/Gizmo.h"
#include "engine/rendering/object/shapes/Object.h"

struct Ray;
struct Vec3;

namespace Scene
{
	void addObject(std::unique_ptr<Object>&& objectPtr);

	void setGizmoToTranslate();
	void setGizmoToScale();
	void setGizmoToRotate();

	void render();
	void handleMouseClicks();
}
