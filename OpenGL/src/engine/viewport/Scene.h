#pragma once

#include <vector>
#include <optional>

#include "engine/rendering/object/Object.h"

struct Ray;
struct Vec3;

namespace Scene
{
	void addObject(Object&& object);

	void clearSelection();
	std::optional<Object*> selectClosestIntersectingObject(const Ray& ray, const Vec3& position);
	Vec3 getSelectionCenter();

	void render();
	void handleMouseClicks();
}
