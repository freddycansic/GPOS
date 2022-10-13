#include "Scene.h"

#include <algorithm>
#include <memory>

#include "Camera.h"
#include "engine/input/Input.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/rendering/object/Object.h"
#include "engine/rendering/object/shapes/Cube.h"


namespace Scene
{
	std::vector<Object> s_Objects;
	std::vector<Object*> s_SelectedObjects;
	std::unique_ptr<Gizmo> sp_Gizmo;

	void addObject(Object&& object)
	{
		s_Objects.push_back(std::move(object));
	}

	void render()
	{
		static size_t i = 0;
		for (auto& object : s_Objects)
		{
			ShapeRenderer::draw(object);
		}

		if (!s_SelectedObjects.empty())
		{
			sp_Gizmo->render(getSelectionCenter());
		}
	}

	void handleMouseClicks()
	{
		if (!Input::isMouseButtonDown(MouseButtons::LEFT)) return;

		const auto& mouseRay = Camera::perspRayFromCameraScreenPos(Input::getMousePos());

		if (s_Objects.empty()) return; // cant select anything from empty scene

		if (!s_SelectedObjects.empty())
		{

			if (const auto& intersectingAxis = sp_Gizmo->getIntersectingHandleAxis(mouseRay); intersectingAxis.has_value())
			{
				for (const auto& object : s_SelectedObjects)
				{	// TODO
					sp_Gizmo->getTransformation(intersectingAxis.value(), 0.001f)(*object);
				}
				return;
			}
		}

		const auto selectedObject = selectClosestIntersectingObject(mouseRay, Camera::getPos());

		// if im not clicking on anything
		if (!selectedObject.has_value())
		{
			clearSelection();
		} else
		{
			sp_Gizmo = std::make_unique<ScaleGizmo>();
		}
	}

	std::optional<Object*> selectClosestIntersectingObject(const Ray& ray, const Vec3& position)
	{
		std::optional<Object*> closest;

		if (s_Objects.empty()) return closest; // no selectable object in empty scene

		float closestDistance = std::numeric_limits<float>::max();

		for (auto& object : s_Objects)
		{
			if (object.shapePtr->getAABB().isRayIntersecting(ray))
			{
				if (const auto& rayIntersection = object.shapePtr->isRayIntersecting(ray); rayIntersection.has_value())
				{
					const auto& pointOfIntersection = rayIntersection.value();

					//const auto distanceFromPointToPlane = std::abs(planeOfIntersection.x * position.x + planeOfIntersection.y * position.y + planeOfIntersection.z * position.z + planeOfIntersection.w) / std::sqrt(planeOfIntersection.x * planeOfIntersection.x + planeOfIntersection.y * planeOfIntersection.y + planeOfIntersection.z * planeOfIntersection.z);

					const auto distanceFromPointToIntersection = std::pow(pointOfIntersection.x - position.x, 2) + std::pow(pointOfIntersection.y - position.y, 2) + std::pow(pointOfIntersection.z - position.z, 2);

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
			std::cout << "SELECTING OBJ" << std::endl;
			closest.value()->selected = true;
			s_SelectedObjects.push_back(closest.value());
		}

		return closest;
	}
	
	void clearSelection()
	{
		for (auto& selectedObject : s_SelectedObjects)
		{
			selectedObject->selected = false;
		}

		s_SelectedObjects.clear();
	}

	Vec3 getSelectionCenter()
	{
		Vec3 center;
		for (const auto& object : s_SelectedObjects)
		{
			center += object->shapePtr->getAvgPosition();
		}

		center /= static_cast<float>(s_SelectedObjects.size());

		return center;
	}
}