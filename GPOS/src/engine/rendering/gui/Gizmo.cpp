#include "Gizmo.h"

#include "engine/Colours.h"
#include "engine/rendering/ObjectRenderer.h"
#include "engine/rendering/objects/Line.h"
#include "engine/rendering/objects/Cube.h"
#include "engine/rendering/objects/Model.h"

GizmoTool s_Tool = GizmoTool::TRANSLATE;

constexpr float GIZMO_LINE_WIDTH = 0.02f;
constexpr float GIZMO_LINE_LENGTH = 1.0f;

std::array s_Lines =
{
	Line(0, 0, 0, -GIZMO_LINE_LENGTH, 0, 0, GIZMO_LINE_WIDTH, Colours::RED),
	Line(0, 0, 0, 0, -GIZMO_LINE_LENGTH, 0, GIZMO_LINE_WIDTH, Colours::GREEN),
	Line(0, 0, 0, 0, 0, GIZMO_LINE_LENGTH, GIZMO_LINE_WIDTH, Colours::BLUE)
};

std::array<Model, 3>& getHandles()
{
	static constexpr float ROTATE_GIZMO_HANDLE_SIZE = 0.08f;
	static const std::string ROTATE_GIZMO_HANDLE_MODEL_PATH = "res/models/gizmo_rotate_handle.obj";
	static std::array s_RotateHandles =
	{
		Model(ROTATE_GIZMO_HANDLE_MODEL_PATH, 0, -1, 0, 0, ROTATE_GIZMO_HANDLE_SIZE, Colours::RED),
		Model(ROTATE_GIZMO_HANDLE_MODEL_PATH, 0, 0, -1, 0, ROTATE_GIZMO_HANDLE_SIZE, Colours::GREEN),
		Model(ROTATE_GIZMO_HANDLE_MODEL_PATH, 0, 0, 0, 1, ROTATE_GIZMO_HANDLE_SIZE, Colours::BLUE)
	};

	static constexpr float TRANSLATE_GIZMO_HANDLE_SIZE = 0.08f;
	static const std::string TRANSLATE_GIZMO_HANDLE_MODEL_PATH = "res/models/gizmo_translate_handle.obj";
	static std::array s_TranslateHandles =
	{
		Model(TRANSLATE_GIZMO_HANDLE_MODEL_PATH, 0, -1, 0, 0, TRANSLATE_GIZMO_HANDLE_SIZE, Colours::RED),
		Model(TRANSLATE_GIZMO_HANDLE_MODEL_PATH, 0, 0, -1, 0, TRANSLATE_GIZMO_HANDLE_SIZE, Colours::GREEN),
		Model(TRANSLATE_GIZMO_HANDLE_MODEL_PATH, 0, 0, 0, 1, TRANSLATE_GIZMO_HANDLE_SIZE, Colours::BLUE)
	};

	static constexpr float SCALE_GIZMO_HANDLE_SIZE = 0.06f;
	static const std::string SCALE_GIZMO_HANDLE_MODEL_PATH = "res/models/cube.obj";
	static std::array s_ScaleHandles =
	{
		Model(SCALE_GIZMO_HANDLE_MODEL_PATH, 0, -1, 0, 0, SCALE_GIZMO_HANDLE_SIZE, Colours::RED),
		Model(SCALE_GIZMO_HANDLE_MODEL_PATH, 0, 0, -1, 0, SCALE_GIZMO_HANDLE_SIZE, Colours::GREEN),
		Model(SCALE_GIZMO_HANDLE_MODEL_PATH, 0, 0, 0, 1, SCALE_GIZMO_HANDLE_SIZE, Colours::BLUE)
	};

	if (static bool first = true; first)
	{
		s_TranslateHandles[0].setRotation(0, 0, 90);
		s_TranslateHandles[1].setRotation(180, 0, 0);
		s_TranslateHandles[2].setRotation(90, 0, 0);
		first = false;
	}

	switch (s_Tool)
	{
		case GizmoTool::TRANSLATE:
			return s_TranslateHandles;

		case GizmoTool::ROTATE:
			return s_RotateHandles;

		case GizmoTool::SCALE:
			return s_ScaleHandles;
	}

	return s_TranslateHandles;
}

namespace Gizmo
{
	void transformObject(Object& object, const Vec3& transformation)
	{
		switch (s_Tool)
		{
			case GizmoTool::TRANSLATE:
			{
				object.offsetTranslation(Vec3(transformation.x, transformation.y, -transformation.z));
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
		using namespace Flags;

		s_Lines[0].setTranslation(pos);
		ObjectRenderer::draw(s_Lines[0], NO_DEPTH_TEST | NO_LIGHTING | ALWAYS_SOLID);

		s_Lines[1].setTranslation(pos);
		ObjectRenderer::draw(s_Lines[1], NO_DEPTH_TEST | NO_LIGHTING | ALWAYS_SOLID);

		s_Lines[2].setTranslation(pos);
		ObjectRenderer::draw(s_Lines[2], NO_DEPTH_TEST | NO_LIGHTING | ALWAYS_SOLID);

		auto& handles = getHandles();

		handles[0].setTranslation(pos.x - GIZMO_LINE_LENGTH, pos.y, pos.z);
		ObjectRenderer::draw(handles[0], NO_DEPTH_TEST | NO_LIGHTING | ALWAYS_SOLID);

		handles[1].setTranslation(pos.x, pos.y - GIZMO_LINE_LENGTH, pos.z);
		ObjectRenderer::draw(handles[1], NO_DEPTH_TEST | NO_LIGHTING | ALWAYS_SOLID);

		handles[2].setTranslation(pos.x, pos.y, pos.z + GIZMO_LINE_LENGTH);
		ObjectRenderer::draw(handles[2], NO_DEPTH_TEST | NO_LIGHTING | ALWAYS_SOLID);
	}

	std::optional<Vec3> getIntersectingHandleAxis(const Ray& ray)
	{
		std::optional<Vec3> axis;
		const auto& handles = getHandles();

		if (handles.at(0).isRayIntersecting(ray)) return axis.emplace(1, 0, 0);
		if (handles.at(1).isRayIntersecting(ray)) return axis.emplace(0, 1, 0);
		if (handles.at(2).isRayIntersecting(ray)) return axis.emplace(0, 0, 1);

		return axis;
	}

	std::optional<Vec3> getHandleIntersectionPoint(const Ray& ray)
	{
		std::optional<Vec3> poi;
		const auto& handles = getHandles();

		if (poi = handles.at(0).isRayIntersecting(ray); poi.has_value()) return poi;
		if (poi = handles.at(1).isRayIntersecting(ray); poi.has_value()) return poi;
		if (poi = handles.at(2).isRayIntersecting(ray); poi.has_value()) return poi;

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