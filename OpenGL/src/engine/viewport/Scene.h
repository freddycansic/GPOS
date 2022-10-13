#pragma once

#include <vector>
#include <optional>

#include "engine/rendering/object/Object.h"

struct Ray;
struct Vec3;

namespace Scene
{
	void addObject(Object&& object);

	[[nodiscard]] const std::vector<Object*>& getSelected();
	void clearSelection();
	std::optional<Object*> selectClosestIntersectingObject(const Ray& ray, const Vec3& position);
	Vec3 getCenter();

	void render();
	void handleObjectSelection();
	void handleGizmoSelection();
}
