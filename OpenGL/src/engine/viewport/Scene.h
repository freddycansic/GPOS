#pragma once

#include <memory>

#include "engine/rendering/gui/Gizmo.h"
#include "engine/rendering/object/shapes/Object.h"

struct Ray;
struct Vec3;

namespace Scene
{
	void addObject(std::unique_ptr<Object>&& objectPtr);
	void duplicateCurrentSelected();

	void setGizmoToTranslate();
	void setGizmoToScale();
	void setGizmoToRotate();

	const std::vector<Object*>& getSelectedObjects();

	void render();
	void handleMouseClicks();
}
