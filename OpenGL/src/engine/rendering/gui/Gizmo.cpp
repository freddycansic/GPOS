#include "Gizmo.h"

#include "engine/Colours.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/rendering/object/Object.h"
#include "engine/rendering/object/shapes/Line.h"
#include "engine/rendering/object/shapes/Cube.h"

constexpr float GIZMO_LINE_WIDTH = 0.01f;

const std::array<Object, 3> Gizmo::s_Lines =
{
	Object(std::make_unique<Line>(0, 0, 0, 1, 0, 0, GIZMO_LINE_WIDTH), Colours::RED),
	Object(std::make_unique<Line>(0, 0, 0, 0, 1, 0, GIZMO_LINE_WIDTH), Colours::GREEN),
	Object(std::make_unique<Line>(0, 0, 0, 0, 0, 1, GIZMO_LINE_WIDTH), Colours::BLUE)
};

void Gizmo::render() const
{
	for (const auto& line : s_Lines)
	{
		ShapeRenderer::draw(line, ShapeRenderer::NO_DEPTH_TEST);
	}

	for (const auto& handle : getHandles())
	{
		ShapeRenderer::draw(handle, ShapeRenderer::NO_DEPTH_TEST);
	}
}

ScaleGizmo::ScaleGizmo(const Vec3& pos)
{
	for (auto& line : s_Lines)
	{
		line.shapePtr->setTranslation(pos.x, pos.y, pos.z);
	}

	for (auto& handle : s_Handles)
	{
		handle.shapePtr->setTranslation(pos.x, pos.y, pos.z);
	}
}

constexpr float SCALE_GIZMO_HANDLE_SIZE = 0.02f;

const std::array<Object, 3> ScaleGizmo::s_Handles =
{
	Object(std::make_unique<Cube>(1, 0, 0, SCALE_GIZMO_HANDLE_SIZE), Colours::RED),
	Object(std::make_unique<Cube>(0, 1, 0, SCALE_GIZMO_HANDLE_SIZE), Colours::GREEN),
	Object(std::make_unique<Cube>(0, 0, 1, SCALE_GIZMO_HANDLE_SIZE), Colours::BLUE)
};

const std::array<Object, 3>& ScaleGizmo::getHandles() const
{
	return s_Handles;
}
