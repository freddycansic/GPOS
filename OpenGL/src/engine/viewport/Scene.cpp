#include "Scene.h"

#include <algorithm>

#include "Camera.h"
#include "engine/input/Input.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/rendering/object/Object.h"
#include "engine/rendering/object/shapes/Cube.h"

std::vector<Object> objects;
std::vector<Object*> selectedObjects;

namespace Scene
{
	std::optional<Object*> selectClosestIntersectingObject(const Ray& ray, const Vec3& position)


	std::unique_ptr<Gizmo> sp_Gizmo;

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

		if (!Scene::getSelected().empty())
		{
			sp_Gizmo->render(Scene::getCenter());
		}
	}

	void handleObjectSelection()
	{
		if (!Input::isMouseButtonDown(MouseButtons::LEFT)) return;

		// TODO fix gross order of precedence stuff
		const auto mouseRay = Camera::perspRayFromCameraScreenPos(Input::getMousePos());

		const auto selectedObject = selectClosestIntersectingObject(mouseRay, Camera::getPos());

		if (!getSelected().empty())
		{
			if (!selectedObject.has_value())
			{
				if (!Input::isKeyDown(Keys::LEFT_CONTROL))
				{
					clearSelection();
				}
			}

			if (sp_Gizmo == nullptr)
			{
				//switch (selectedTool)
				//{
				//case Tool::Translate:
				//	sp_Gizmo = std::make_unique<TranslateGizmo>();
				//}

				sp_Gizmo = std::make_unique<ScaleGizmo>();
			}
		}
	}

	void handleGizmoSelection()
	{
		if (!Input::isMouseButtonDown(MouseButtons::LEFT)) return;

		if (const auto& intersectingAxis = sp_Gizmo->getIntersectingHandleAxis(mouseRay); intersectingAxis.has_value())
		{
			//std::ranges::transform(selectedObjects, 
			//std::transform(selectedObjects.begin(), selectedObjects.end(), 
			//	[&intersectingAxis](Object* object)
			//	{
			//		sp_Gizmo->getTransformation(intersectingAxis.value(), 1.1f)(*object);
			//	});

			for (const auto& object : selectedObjects)
			{
				sp_Gizmo->getTransformation(intersectingAxis.value(), 1.1f)(*object);
			}
		}
	}

	std::optional<Object*> selectClosestIntersectingObject(const Ray& ray, const Vec3& position)
	{
		std::optional<Object*> closest;

		if (objects.empty()) return closest; // no selectable object in empty scene

		for (auto& object : objects)
		{
			float closestDistance = std::numeric_limits<float>::max();

			if (object.shapePtr->getAABB().isRayIntersecting(ray))
			{
				const auto rayIntersection = object.shapePtr->isRayIntersecting(ray);
				if (rayIntersection)
				{
					const auto& pointOfIntersection = rayIntersection.value();

					//const auto distanceFromPointToPlane = std::abs(planeOfIntersection.x * position.x + planeOfIntersection.y * position.y + planeOfIntersection.z * position.z + planeOfIntersection.w) / std::sqrt(planeOfIntersection.x * planeOfIntersection.x + planeOfIntersection.y * planeOfIntersection.y + planeOfIntersection.z * planeOfIntersection.z);

					const auto distanceFromPointToIntersection = pointOfIntersection.x * pointOfIntersection.x + pointOfIntersection.y * pointOfIntersection.y + pointOfIntersection.z * pointOfIntersection.z;

					if (distanceFromPointToIntersection < closestDistance)
					{
						closestDistance = distanceFromPointToIntersection;
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

	Vec3 getCenter()
	{
		Vec3 center;
		for (const auto& object : objects)
		{
			center += object.shapePtr->getAvgPosition();
		}

		center /= objects.size();

		return center;
	}
}