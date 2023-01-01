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
	void loadModelIntoScene(const char* path);

	void selectObject(Object* obj);
	void clearSelection();

	const std::vector<Object*>& getSelectedObjects();
	const std::vector<std::unique_ptr<Object>>& getObjects();

	void render();
	void handleMouseClicks();
}
