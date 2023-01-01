#include "Gizmo.h"

#include "engine/Colours.h"
#include "engine/rendering/ObjectRenderer.h"
#include "engine/rendering/objects/Line.h"
#include "engine/rendering/objects/Cube.h"
#include "engine/rendering/objects/Model.h"

GizmoTool s_Tool = GizmoTool::TRANSLATE;

constexpr float GIZMO_LINE_WIDTH = 0.02f;
constexpr float GIZMO_LINE_LENGTH = 1.0f;

std::array<Line, 3> s_Lines =
{
	Line(0, 0, 0, -GIZMO_LINE_LENGTH, 0, 0, GIZMO_LINE_WIDTH, Colours::RED),
	Line(0, 0, 0, 0, -GIZMO_LINE_LENGTH, 0, GIZMO_LINE_WIDTH, Colours::GREEN),
	Line(0, 0, 0, 0, 0, GIZMO_LINE_LENGTH, GIZMO_LINE_WIDTH, Colours::BLUE)
};

constexpr float SCALE_GIZMO_HANDLE_SIZE = 0.08f;
std::array<std::unique_ptr<Object>, 3> s_ScaleHandles =
{
	std::make_unique<Cube>(-1, 0, 0, SCALE_GIZMO_HANDLE_SIZE, Colours::RED),
	std::make_unique<Cube>(0, -1, 0, SCALE_GIZMO_HANDLE_SIZE, Colours::GREEN),
	std::make_unique<Cube>(0, 0, 1, SCALE_GIZMO_HANDLE_SIZE, Colours::BLUE)
};

constexpr float TRANSLATE_GIZMO_HANDLE_SIZE = 0.08f;
std::array<std::unique_ptr<Object>, 3> s_TranslateHandles =
{
	std::make_unique<Cube>(-1, 0, 0, TRANSLATE_GIZMO_HANDLE_SIZE, Colours::RED),
	std::make_unique<Cube>(0, -1, 0, TRANSLATE_GIZMO_HANDLE_SIZE, Colours::GREEN),
	std::make_unique<Cube>(0, 0, 1, TRANSLATE_GIZMO_HANDLE_SIZE, Colours::BLUE)
};

constexpr float ROTATE_GIZMO_HANDLE_SIZE = 0.08f;
std::array<std::unique_ptr<Object>, 3> s_RotateHandles =
{
	std::make_unique<Cube>(-1, 0, 0, ROTATE_GIZMO_HANDLE_SIZE, Colours::RED),
	std::make_unique<Cube>(0, -1, 0, ROTATE_GIZMO_HANDLE_SIZE, Colours::GREEN),
	std::make_unique<Cube>(0, 0, 1, ROTATE_GIZMO_HANDLE_SIZE, Colours::BLUE)
};

const std::array<std::unique_ptr<Object>, 3>& getHandles()
{
	switch (s_Tool)
	{
		case GizmoTool::TRANSLATE:
			return s_TranslateHandles;

		case GizmoTool::ROTATE:
			return s_RotateHandles;

		case GizmoTool::SCALE:
			return s_ScaleHandles;
	}
}

namespace Gizmo
{
	void transformObject(Object& object, const Vec3& transformation)
	{
		switch (s_Tool)
		{
			case GizmoTool::TRANSLATE:
			{
				object.offsetTranslation(transformation);
				break;
			}

			case GizmoTool::ROTATE:
			{
				static constexpr float ROTATION_SENSITIVITY = 30.0f;

				object.offsetRotation(transformation * ROTATION_SENSITIVITY);
				break;
			}

			case GizmoTool::SCALE:
			{
				object.offsetScale(transformation * -1);
				break;
			}
		}
	}

	void render(float x, float y, float z) { render(Vec3(x, y, z)); }

	void render(const Vec3& pos)
	{
		// TODO fix
		using namespace Flags;

		//s_Lines[0].p1 = pos;
		//s_Lines[0].p2 = { pos.x - GIZMO_LINE_LENGTH, pos.y, pos.z };
		s_Lines[0].setTranslation(pos);
		ObjectRenderer::draw(s_Lines[0], NO_DEPTH_TEST | NO_LIGHTING | ALWAYS_SOLID);

		//s_Lines[1].p1 = pos;
		//s_Lines[1].p2 = { pos.x, pos.y - GIZMO_LINE_LENGTH, pos.z };
		s_Lines[1].setTranslation(pos);
		ObjectRenderer::draw(s_Lines[1], NO_DEPTH_TEST | NO_LIGHTING | ALWAYS_SOLID);

		//s_Lines[2].p1 = pos;
		//s_Lines[2].p2 = { pos.x, pos.y, pos.z + GIZMO_LINE_LENGTH };
		s_Lines[2].setTranslation(pos);
		ObjectRenderer::draw(s_Lines[2], NO_DEPTH_TEST | NO_LIGHTING | ALWAYS_SOLID);

		const auto& handles = getHandles();

		handles[0]->setTranslation(pos.x - GIZMO_LINE_LENGTH, pos.y, pos.z);
		ObjectRenderer::draw(*handles[0], NO_DEPTH_TEST | NO_LIGHTING | ALWAYS_SOLID);

		handles[1]->setTranslation(pos.x, pos.y - GIZMO_LINE_LENGTH, pos.z);
		ObjectRenderer::draw(*handles[1], NO_DEPTH_TEST | NO_LIGHTING | ALWAYS_SOLID);

		handles[2]->setTranslation(pos.x, pos.y, pos.z + GIZMO_LINE_LENGTH);
		ObjectRenderer::draw(*handles[2], NO_DEPTH_TEST | NO_LIGHTING | ALWAYS_SOLID);
	}

	std::optional<Vec3> getIntersectingHandleAxis(const Ray& ray)
	{
		std::optional<Vec3> axis;
		const auto& handles = getHandles();

		if (handles.at(0)->isRayIntersecting(ray)) return axis.emplace(1, 0, 0);
		if (handles.at(1)->isRayIntersecting(ray)) return axis.emplace(0, 1, 0);
		if (handles.at(2)->isRayIntersecting(ray)) return axis.emplace(0, 0, 1);

		return axis;
	}

	std::optional<Vec3> getHandleIntersectionPoint(const Ray& ray)
	{
		std::optional<Vec3> poi;
		const auto& handles = getHandles();

		if (poi = handles.at(0)->isRayIntersecting(ray); poi.has_value()) return poi;
		if (poi = handles.at(1)->isRayIntersecting(ray); poi.has_value()) return poi;
		if (poi = handles.at(2)->isRayIntersecting(ray); poi.has_value()) return poi;

		return poi;
	}

	void setTool(GizmoTool tool)
	{
		s_Tool = tool;
	}

	[[nodiscard]] GizmoTool getTool()
	{
		return s_Tool;
	}
}