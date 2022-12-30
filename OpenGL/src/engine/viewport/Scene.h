#pragma once

#include <memory>

#include "engine/rendering/gui/Gizmo.h"
#include "engine/rendering/objects/Object.h"

struct Ray;
struct Vec3;

namespace Scene
{
	void addObject(std::unique_ptr<Object>&& objectPtr);
	void duplicateCurrentSelected();
	void loadModel(const char* path);

	void setGizmoToTranslate();
	void setGizmoToScale();
	void setGizmoToRotate();

	const std::vector<Object*>& getSelectedObjects();

	void render();
	void handleMouseClicks();
}
