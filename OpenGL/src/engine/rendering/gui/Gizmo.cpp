#include "Gizmo.h"

#include "engine/Colours.h"
#include "engine/rendering/ObjectRenderer.h"
#include "engine/rendering/object/shapes/Line.h"
#include "engine/rendering/object/shapes/Cube.h"

constexpr float GIZMO_LINE_WIDTH = 0.02f;
constexpr float GIZMO_LINE_LENGTH = 1.0f;

std::array<Line, 3> Gizmo::s_Lines =
{
	Line(0, 0, 0, -GIZMO_LINE_LENGTH, 0, 0, GIZMO_LINE_WIDTH, Colours::RED),
	Line(0, 0, 0, 0, -GIZMO_LINE_LENGTH, 0, GIZMO_LINE_WIDTH, Colours::GREEN),
	Line(0, 0, 0, 0, 0, GIZMO_LINE_LENGTH, GIZMO_LINE_WIDTH, Colours::BLUE)
};

void Gizmo::render(const Vec3& pos) const
{
	using namespace Flags;

	//s_Lines[0].p1 = pos;
	//s_Lines[0].p2 = { pos.x - GIZMO_LINE_LENGTH, pos.y, pos.z };
	s_Lines[0].setTranslation(pos);
	ObjectRenderer::draw(s_Lines[0], NO_DEPTH_TEST | NO_LIGHTING);

	//s_Lines[1].p1 = pos;
	//s_Lines[1].p2 = { pos.x, pos.y - GIZMO_LINE_LENGTH, pos.z };
	s_Lines[1].setTranslation(pos);
	ObjectRenderer::draw(s_Lines[1], NO_DEPTH_TEST | NO_LIGHTING);

	//s_Lines[2].p1 = pos;
	//s_Lines[2].p2 = { pos.x, pos.y, pos.z + GIZMO_LINE_LENGTH };
	s_Lines[2].setTranslation(pos);
	ObjectRenderer::draw(s_Lines[2], NO_DEPTH_TEST | NO_LIGHTING);

	const auto& handles = this->getHandles();

	handles[0]->setTranslation(pos.x - GIZMO_LINE_LENGTH, pos.y, pos.z);
	ObjectRenderer::draw(*handles[0], NO_DEPTH_TEST | NO_LIGHTING);

	handles[1]->setTranslation(pos.x, pos.y - GIZMO_LINE_LENGTH, pos.z);
	ObjectRenderer::draw(*handles[1], NO_DEPTH_TEST | NO_LIGHTING);

	handles[2]->setTranslation(pos.x, pos.y, pos.z + GIZMO_LINE_LENGTH);
	ObjectRenderer::draw(*handles[2], NO_DEPTH_TEST | NO_LIGHTING);
}

std::optional<Vec3> Gizmo::getIntersectingHandleAxis(const Ray& ray) const
{
	std::optional<Vec3> axis;
	const auto& handles = getHandles();

	if (handles.at(0)->isRayIntersecting(ray)) return axis.emplace(1, 0, 0);
	if (handles.at(1)->isRayIntersecting(ray)) return axis.emplace(0, 1, 0);
	if (handles.at(2)->isRayIntersecting(ray)) return axis.emplace(0, 0, 1);

	return axis;
}

std::optional<Vec3> Gizmo::getHandleIntersectionPoint(const Ray& ray) const
{
	std::optional<Vec3> poi;
	const auto& handles = getHandles();

	if (poi = handles.at(0)->isRayIntersecting(ray); poi.has_value()) return poi;
	if (poi = handles.at(1)->isRayIntersecting(ray); poi.has_value()) return poi;
	if (poi = handles.at(2)->isRayIntersecting(ray); poi.has_value()) return poi;

	return poi;
}

// Scaling gizmo
constexpr float SCALE_GIZMO_HANDLE_SIZE = 0.08f;

std::array<std::unique_ptr<Object>, 3> ScaleGizmo::s_Handles =
{
	std::make_unique<Cube>(-1, 0, 0, SCALE_GIZMO_HANDLE_SIZE, Colours::RED),
	std::make_unique<Cube>(0, -1, 0, SCALE_GIZMO_HANDLE_SIZE, Colours::GREEN),
	std::make_unique<Cube>(0, 0, 1, SCALE_GIZMO_HANDLE_SIZE, Colours::BLUE)
};

std::function<void(Object&)> ScaleGizmo::getOffsetTransformation(const Vec3& transformation) const
{
	return { [&transformation](Object& obj)
	{
		obj.offsetScale(transformation);
	}};
}

const std::array<std::unique_ptr<Object>, 3>& ScaleGizmo::getHandles() const
{
	return s_Handles;
}

// Translating gizmo
constexpr float TRANSLATE_GIZMO_HANDLE_SIZE = 0.08f;

// TODO cone
std::array<std::unique_ptr<Object>, 3> TranslateGizmo::s_Handles =
{
	std::make_unique<Cube>(-1, 0, 0, TRANSLATE_GIZMO_HANDLE_SIZE, Colours::RED),
	std::make_unique<Cube>(0, -1, 0, TRANSLATE_GIZMO_HANDLE_SIZE, Colours::GREEN),
	std::make_unique<Cube>(0, 0, 1, TRANSLATE_GIZMO_HANDLE_SIZE, Colours::BLUE)
};

std::function<void(Object&)> TranslateGizmo::getOffsetTransformation(const Vec3& transformation) const
{
	return { [&transformation](Object& obj)
	{
		obj.offsetTranslation(transformation);
	}};
}

const std::array<std::unique_ptr<Object>, 3>& TranslateGizmo::getHandles() const
{
	return s_Handles;
}

// Rotating gizmo
constexpr float ROTATE_GIZMO_HANDLE_SIZE = 0.08f;

// TODO help
std::array<std::unique_ptr<Object>, 3> RotateGizmo::s_Handles =
{
	std::make_unique<Cube>(-1, 0, 0, ROTATE_GIZMO_HANDLE_SIZE, Colours::RED),
	std::make_unique<Cube>(0, -1, 0, ROTATE_GIZMO_HANDLE_SIZE, Colours::GREEN),
	std::make_unique<Cube>(0, 0, 1, ROTATE_GIZMO_HANDLE_SIZE, Colours::BLUE)
};

std::function<void(Object&)> RotateGizmo::getOffsetTransformation(const Vec3& transformation) const
{
	return { [&transformation](Object& obj)
	{
		obj.offsetRotation(transformation);
	} };
}

const std::array<std::unique_ptr<Object>, 3>& RotateGizmo::getHandles() const
{
	return s_Handles;
}