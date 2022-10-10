#pragma once
#include <vector>

struct Object;
struct Ray;
struct Vec3;

namespace Scene
{
	void addObject(Object&& object);

	[[nodiscard]] const std::vector<Object*>& getSelected();
	[[nodiscard]] Object& getClosestIntersectingObject(const Ray& ray, const Vec3& position);

	void render();
}
