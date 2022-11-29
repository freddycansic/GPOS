#include "Scene.h"

#include <memory>
#include <optional>
#include <algorithm>

#include "Camera.h"
#include "engine/input/Input.h"
#include "engine/rendering/ObjectRenderer.h"
#include "engine/rendering/Renderer.h"
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

				const auto distanceFromPointToIntersection = std::pow(pointOfIntersection.x - position.x, 2) + std::pow(pointOfIntersection.y - position.y, 2) + std::pow(pointOfIntersection.z - position.z, 2);

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

void selectObject(Object* obj)
{
	s_SelectedObjects.push_back(obj);
	obj->selected = true;
}

namespace Scene
{
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
	{ // TODO make this readable
		static bool s_UsingGizmo = false;
		static Vec2 s_FirstMousePos;
		static std::optional<Vec3> s_IntersectingAxis;
		static Vec2 s_CentreToFirstMouseScreen, s_SelectionCentreScreen;

		if (s_UsingGizmo && Input::isMouseButtonDown(MouseButtons::LEFT))
		{
			const auto mousePos = Input::getMousePos();

			const auto centreToCurrentMouse = mousePos - s_SelectionCentreScreen;

			const auto mag = s_CentreToFirstMouseScreen.dot(centreToCurrentMouse) / std::pow(s_CentreToFirstMouseScreen.magnitude(), 2);

			for (const auto& object : s_SelectedObjects)
			{
				sp_Gizmo->getOffsetTransformation(s_IntersectingAxis.value() * -1 * (mag - 1))(*object);
			}

			return;
		}

		if (s_UsingGizmo && Input::isMouseButtonJustReleased(MouseButtons::LEFT))
		{
			for (const auto& object : s_SelectedObjects)
			{
				object->applyOffset();
			}

			s_UsingGizmo = false;

			return;
		}

		if (!Input::isMouseButtonDown(MouseButtons::LEFT)) return;
		if (s_Objects.empty()) return; // cant select anything from empty scene

		const auto& mouseRay = Camera::perspRayFromCameraScreenPos(Input::getMousePos());

		if (!s_SelectedObjects.empty())
		{
			if (s_IntersectingAxis = sp_Gizmo->getIntersectingHandleAxis(mouseRay); s_IntersectingAxis.has_value())
			{
				if (!s_UsingGizmo) 
				{
					s_FirstMousePos = Input::getMousePos();

					const auto& centre = getSelectionCenter();
					const Vec4 projectedCentre = Vec4(centre, 1.0f) * Renderer::getViewProjectionMatrix();
					const Vec2 flattenedCentreNDC = { projectedCentre.x / projectedCentre.w, projectedCentre.y / projectedCentre.w };

					s_SelectionCentreScreen = Camera::NDCToScreenBounds(flattenedCentreNDC);
					s_CentreToFirstMouseScreen = s_FirstMousePos - s_SelectionCentreScreen; // TODO only get once at first click
				}

				s_UsingGizmo = true;

				return;
			}
		}

		const auto closestObject = findClosestIntersectingObject(mouseRay, Camera::getPos());

		if (!Input::isKeyDown(Keys::LEFT_CONTROL))
		{
			clearSelection();
		}

		if (closestObject.has_value())
		{
			selectObject(closestObject.value());

			if (sp_Gizmo == nullptr) sp_Gizmo = std::make_unique<TranslateGizmo>();
		}
	}

	void duplicateCurrentSelected()
	{
		const auto currentSelectionCopy = s_SelectedObjects;
		clearSelection();

		for (const auto& object : currentSelectionCopy)
		{
			//s_Objects.push_back(); // TODO make deep copy of unique ptr
		}
	}

	void addObject(std::unique_ptr<Object>&& objectPtr)
	{
		s_Objects.push_back(std::move(objectPtr));
	}

	void setGizmoToTranslate()
	{
		sp_Gizmo = std::make_unique<TranslateGizmo>();
	}

	void setGizmoToScale()
	{
		sp_Gizmo = std::make_unique<ScaleGizmo>();
	}

	void setGizmoToRotate()
	{
		sp_Gizmo = std::make_unique<RotateGizmo>();
	}

	const std::vector<Object*>& getSelectedObjects()
	{
		return s_SelectedObjects;
	}
}