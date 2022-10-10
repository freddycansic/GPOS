#include "Scene.h"

#include "engine/rendering/ShapeRenderer.h"
#include "engine/rendering/object/Object.h"
#include "engine/rendering/object/shapes/Cube.h"

std::vector<Object> objects;
std::vector<Object*> selected;

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

	Object& getClosestIntersectingObject(const Ray& ray, const Vec3& position)
	{
		std::vector<Object*> intersectingObjects;

		for (auto& object : objects)
		{
			if (object.shapePtr->getAABB().isRayIntersecting(ray))
			{
				if (object.shapePtr->isRayIntersecting(ray))
				{
					intersectingObjects.push_back(&object);
				} 
			}
		}

		Object* closest = nullptr;

		for (const auto& object : intersectingObjects)
		{
			static size_t closestDistance = std::numeric_limits<size_t>::max();
		}

		return *closest;
	}

	const std::vector<Object*>& getSelected()
	{
		return selected;
	}
}