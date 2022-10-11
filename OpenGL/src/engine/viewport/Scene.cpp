#include "Scene.h"

#include "engine/rendering/ShapeRenderer.h"
#include "engine/rendering/object/Object.h"
#include "engine/rendering/object/shapes/Cube.h"

std::vector<Object> objects;
std::vector<Object*> selectedObjects;

namespace Scene
{
	void addObject(Object&& object)
	{
		objects.push_back(std::move(object));
	}

	void render()
	{
		for (auto& object : objects)
		{
			ShapeRenderer::draw(object);
		}
	}

	std::optional<Object*> selectClosestIntersectingObject(const Ray& ray, const Vec3& position)
	{
		std::optional<Object*> closest;

		if (objects.empty()) return closest; // no selectable object in empty scene

		for (auto& object : objects)
		{
			//if (!object.shapePtr->selectable()) continue; // cannot select unselectable object

			static float closestDistance = std::numeric_limits<float>::max();

			if (object.shapePtr->getAABB().isRayIntersecting(ray))
			{
				const auto rayIntersection = object.shapePtr->isRayIntersecting(ray);
				if (rayIntersection)
				{
					const auto& planeOfIntersection = rayIntersection.value();

					const auto distanceFromPointToPlane = std::abs(planeOfIntersection.x * position.x + planeOfIntersection.y * position.y + planeOfIntersection.z * position.z + planeOfIntersection.w) / std::sqrt(planeOfIntersection.x * planeOfIntersection.x + planeOfIntersection.y * planeOfIntersection.y + planeOfIntersection.z * planeOfIntersection.z);

					if (distanceFromPointToPlane < closestDistance)
					{
						closestDistance = distanceFromPointToPlane;
						closest.emplace(&object);
					}
				}
			}
		}

		if (closest.has_value())
		{
			closest.value()->selected = true;
			selectedObjects.push_back(closest.value());
		}

		return closest;
	}

	const std::vector<Object*>& getSelected()
	{
		return selectedObjects;
	}

	void clearSelection()
	{
		for (auto& selectedObject : selectedObjects)
		{
			selectedObject->selected = false;
		}

		selectedObjects.clear();
	}
}