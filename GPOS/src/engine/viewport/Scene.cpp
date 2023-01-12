#include "Scene.h"

#include <memory>
#include <optional>
#include <algorithm>
#include <unordered_map>

#include "Camera.h"
#include "engine/Debug.h"
#include "engine/input/Buttons.h"
#include "engine/input/Files.h"
#include "engine/input/Input.h"
#include "engine/rendering/ObjectRenderer.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/objects/Cube.h"
#include "engine/rendering/objects/Model.h"
#include "engine/rendering/ObjectRenderer.h"
#include "engine/Colours.h"

std::vector<std::unique_ptr<Object>> s_Objects;
std::vector<Object*> s_SelectedObjects;

Vec3 getSelectionCenter()
{
	Vec3 center = {0, 0, 0};
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

				const auto distanceFromPointToIntersection = static_cast<float>(std::pow(pointOfIntersection.x - position.x, 2)) + static_cast<float>(std::pow(pointOfIntersection.y - position.y, 2)) + static_cast<float>(std::pow(pointOfIntersection.z - position.z, 2));

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

std::vector<Line> gridLines;
constexpr float GRIDLINE_LENGTH = 200.0f;
constexpr float AXES_LINE_WIDTH = 0.01f;

void drawAxes()
{
	using namespace Flags;

	static Line x(-GRIDLINE_LENGTH / 2, 0, 0, GRIDLINE_LENGTH / 2, 0, 0, AXES_LINE_WIDTH, Colours::RED);
	static Line z(0, 0, -GRIDLINE_LENGTH / 2, 0, 0, GRIDLINE_LENGTH / 2, AXES_LINE_WIDTH, Colours::BLUE);
	static Cube centreCube(0, 0, 0, 0.05f, Colours::WHITE);

	ObjectRenderer::draw(centreCube, NO_LIGHTING | ALWAYS_SOLID);
	ObjectRenderer::draw(x, NO_LIGHTING | ALWAYS_SOLID);
	ObjectRenderer::draw(z, NO_LIGHTING | ALWAYS_SOLID);

	for (auto& gridLine : gridLines)
	{
		ObjectRenderer::draw(gridLine, NO_LIGHTING | ALWAYS_SOLID);
	}
}

namespace Scene
{
	void init()
	{
		static constexpr float GRIDLINE_STEP = 2.5f;
		static constexpr float GRIDLINE_WIDTH = 0.005f;
		for (float i = -GRIDLINE_LENGTH / 2; i < GRIDLINE_LENGTH / 2; i += GRIDLINE_STEP)
		{
			gridLines.emplace_back
			(
				-GRIDLINE_LENGTH / 2, 0, i, 
				GRIDLINE_LENGTH / 2, 0, i,
				GRIDLINE_WIDTH, Colours::DEFAULT
			);

			gridLines.emplace_back
			(
				i, 0, -GRIDLINE_LENGTH / 2,
				i, 0, GRIDLINE_LENGTH / 2,
				GRIDLINE_WIDTH, Colours::DEFAULT
			);
		}
	}

	std::unordered_map<const char*, Texture> atlas;
	const Texture& getTexture(const char* path)
	{
		if (atlas.contains(path)) return atlas.at(path);

		atlas[path] = std::move(Texture(path));

		return atlas.at(path);
	}

	void saveToFile()
	{
		
	}

	void clearSelection()
	{
		for (const auto& selectedObject : s_SelectedObjects)
		{
			selectedObject->selected = false;
		}

		s_SelectedObjects.clear();
	}

	void selectAll()
	{
		for (const auto& object : s_Objects)
		{
			selectObject(&*object);
		}
	}

	void selectObject(Object* obj)
	{
		if (obj->selected) return;

		s_SelectedObjects.push_back(obj);
		obj->selected = true;
	}

	void deleteSelected()
	{
		std::vector<std::unique_ptr<Object>> newObjects;

		for (auto& object : s_Objects)
		{
			if (!object->selected)
			{
				newObjects.push_back(std::move(object));
			}
		}

		s_SelectedObjects.clear(); // dont call clearSelection cause pointers will be invalid

		s_Objects = std::move(newObjects);
	}

	void invertSelection()
	{
		s_SelectedObjects.clear();

		for (const auto& object : s_Objects)
		{
			if (object->selected)
			{
				object->selected = false;
			} else
			{
				selectObject(&*object);
			}
		}
	}

	void render()
	{
		drawAxes();

		for (const auto& object : s_Objects)
		{
			ObjectRenderer::draw(*object);
		}

		if (!s_SelectedObjects.empty())
		{
			Gizmo::render(getSelectionCenter());
		}
	}

	void handleMouseClicks()
	{ // TODO make this readable
		static bool s_UsingGizmo = false;
		static Vec2 s_FirstMousePos;
		static std::optional<Vec3> s_IntersectingAxis;
		static Vec2 s_CentreToFirstMouseScreen, s_SelectionCentreScreen;

		// TODO add cancelling gizmo movements

		if (s_UsingGizmo && MouseButtons::MOUSE_1->isDown())
		{
			const auto mousePos = Input::getMousePos();

			const auto centreToCurrentMouse = mousePos - s_SelectionCentreScreen;

			const auto mag = s_CentreToFirstMouseScreen.dot(centreToCurrentMouse) / static_cast<float>(std::pow(s_CentreToFirstMouseScreen.magnitude(), 2));

			for (const auto& object : s_SelectedObjects)
			{
				Gizmo::transformObject(*object, s_IntersectingAxis.value() * -1 * (mag - 1));
			}

			return;
		}

		if (s_UsingGizmo && MouseButtons::MOUSE_1->isJustReleased())
		{
			for (const auto& object : s_SelectedObjects)
			{
				object->applyOffset();
			}

			s_UsingGizmo = false;

			return;
		}

		if (!MouseButtons::MOUSE_1->isDown()) return;
		if (s_Objects.empty()) return; // cant select anything from empty scene

		const auto& mouseRay = Camera::perspRayFromCameraScreenPos(Input::getMousePos());

		if (!s_SelectedObjects.empty())
		{
			if (s_IntersectingAxis = Gizmo::getIntersectingHandleAxis(mouseRay); s_IntersectingAxis.has_value())
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

		if (!Keys::LEFT_CONTROL->isDown())
		{
			clearSelection();
		}

		if (closestObject.has_value())
		{
			selectObject(closestObject.value());
		}
	}

	void duplicateCurrentSelected()
	{
		for (const auto& object : s_SelectedObjects)
		{
			s_Objects.push_back(object->clone());

			// deselect old object
			s_Objects.at(s_Objects.size() - 1)->selected = false;
		}
	}

	void loadModelIntoScene()
	{
		if (const auto path = Files::getPathFromDialogue("amf,3ds,ac,ase,assbin,b3d,bvh,collada,dxf,csm,hmp,irrmesh,iqm,irr,lwo,lws,m3d,md2,md3,md5,mdc,mdl,nff,ndo,off,obj,ogre,opengex,ply,ms3d,cob,blend,ifc,xgl,fbx,q3d"); path)
		{
			Model::loadModelMeshes(path);
			
			const auto& models = Model::meshes.at(path);
				
			for (size_t modelIdx = 0; modelIdx < models.size(); ++modelIdx)
			{
				addObject(std::make_unique<Model>(0, 0, 0, path, modelIdx, models.at(modelIdx).second));
			}
		}
	}

	void addObject(std::unique_ptr<Object>&& objectPtr)
	{
		s_Objects.push_back(std::move(objectPtr));
	}

	const std::vector<Object*>& getSelectedObjects()
	{
		return s_SelectedObjects;
	}
	const std::vector<std::unique_ptr<Object>>& getObjects()
	{
		return s_Objects;
	}

	Vec3 s_BackgroundColour = { 0, 0, 0 };
	Vec3& getMutRefBackgroundColour()
	{
		return s_BackgroundColour;
	}

	void setBackgroundColour(const Vec3& colour)
	{
		s_BackgroundColour = colour;
	}
}