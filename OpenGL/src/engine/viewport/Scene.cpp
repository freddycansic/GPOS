#include "Scene.h"

#include <memory>
#include <optional>

#include "Camera.h"
#include "engine/input/Input.h"
#include "engine/rendering/ObjectRenderer.h"
#include "engine/rendering/object/shapes/Cube.h"

std::vector<std::unique_ptr<Object>> s_Objects;
std::vector<Object*> s_SelectedObjects;
std::unique_ptr<Gizmo> sp_Gizmo;

void clearSelection()
{
	for (const auto& selectedObject : s_SelectedObjects)
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
		center += object->getAvgPosition();
	}

	center /= static_cast<float>(s_SelectedObjects.size());

	return center;
}

std::optional<Object*> findClosestIntersectingObject(const Ray& ray, const Vec3& position)
{
	std::optional<Object*> closest;

	if (s_Objects.empty()) return closest; // no selectable object in empty scene

	float closestDistance = std::numeric_limits<float>::max();

	for (auto& object : s_Objects)
	{
		if (object->getAABB().isRayIntersecting(ray))
		{
			if (const auto& rayIntersection = object->isRayIntersecting(ray); rayIntersection.has_value())
			{
				const auto& pointOfIntersection = rayIntersection.value();

				//const auto distanceFromPointToPlane = std::abs(planeOfIntersection.x * position.x + planeOfIntersection.y * position.y + planeOfIntersection.z * position.z + planeOfIntersection.w) / std::sqrt(planeOfIntersection.x * planeOfIntersection.x + planeOfIntersection.y * planeOfIntersection.y + planeOfIntersection.z * planeOfIntersection.z);

				const auto distanceFromPointToIntersection = std::powf(pointOfIntersection.x - position.x, 2) + std::powf(pointOfIntersection.y - position.y, 2) + std::powf(pointOfIntersection.z - position.z, 2);

				if (distanceFromPointToIntersection < closestDistance)
				{
					closestDistance = distanceFromPointToIntersection;
					closest.emplace(&*object); // dereference unique ptr, get regular pointer
				}
			}
		}
	}

	return closest;
}

namespace Scene
{
	void addObject(std::unique_ptr<Object>&& objectPtr)
	{
		s_Objects.push_back(std::move(objectPtr));
	}

	void render()
	{
		for (const auto& object : s_Objects)
		{
			ObjectRenderer::draw(*object);
		}

		if (!s_SelectedObjects.empty())
		{
			sp_Gizmo->render(getSelectionCenter());
		}
	}

	void handleMouseClicks()
	{
		if (!Input::isMouseButtonDown(MouseButtons::LEFT)) return;

		if (s_Objects.empty()) return; // cant select anything from empty scene

		const auto& mouseRay = Camera::perspRayFromCameraScreenPos(Input::getMousePos());

		//if (Input::isMouseButtonJustDown(MouseButtons::LEFT)) s_FirstMousePos = Input::getMousePos();

		//static std::optional<Vec2> s_FirstMousePos;

		if (!s_SelectedObjects.empty())
		{
			//if (const auto& intersectingAxis = sp_Gizmo->getIntersectingHandleAxis(mouseRay); intersectingAxis.has_value())
			//{
				//if (!s_FirstMousePos.has_value()) s_FirstMousePos = Input::getMousePos(); // if its the first time clicking obj get mouse pos

				//for (const auto& object : s_SelectedObjects)
				//{
				//	const auto mousePos = Input::getMousePos();

				//	const auto mousePosDifference = s_FirstMousePos.value() - mousePos;
				//	const auto mousePosDifferenceMagnitude = mousePosDifference.magnitude();

				//	const auto direction = s_FirstMousePos.value().dot(mousePos) < 0.0f ? 1.0f : -1.0f;

				//	static constexpr float SENSITIVITY = 0.1f;
				//	sp_Gizmo->getOffsetTransformation(intersectingAxis.value(), SENSITIVITY * mousePosDifferenceMagnitude * direction)(*object);
				//}
				//return;
			//}
		}

		//s_FirstMousePos = std::optional<Vec2>(); // reset first mouse pos

		const auto closestObject = findClosestIntersectingObject(mouseRay, Camera::getPos());

		if (!Input::isKeyDown(Keys::LEFT_CONTROL))
		{
			clearSelection();
		}

		if (closestObject.has_value())
		{
			closestObject.value()->selected = true;
			s_SelectedObjects.push_back(closestObject.value());

			// TODO
			if (sp_Gizmo == nullptr) sp_Gizmo = std::make_unique<TranslateGizmo>();
		}
	}
}