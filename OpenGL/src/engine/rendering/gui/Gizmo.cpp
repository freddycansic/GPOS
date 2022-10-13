#include "Gizmo.h"

#include "engine/Colours.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/rendering/object/Object.h"
#include "engine/rendering/object/shapes/Line.h"
#include "engine/rendering/object/shapes/Cube.h"

constexpr float GIZMO_LINE_WIDTH = 0.02f;

const std::array<Object, 3> Gizmo::s_Lines =
{
	Object(std::make_unique<Line>(0, 0, 0, -1, 0, 0, GIZMO_LINE_WIDTH), Colours::RED),
	Object(std::make_unique<Line>(0, 0, 0, 0, -1, 0, GIZMO_LINE_WIDTH), Colours::GREEN),
	Object(std::make_unique<Line>(0, 0, 0, 0, 0, 1, GIZMO_LINE_WIDTH), Colours::BLUE)
};

void Gizmo::render(const Vec3& pos) const
{
	for (const auto& line : s_Lines)
	{
		line.shapePtr->setTranslation(pos.x, pos.y, pos.z);
		ShapeRenderer::draw(line, ShapeRenderer::NO_DEPTH_TEST);
	}

	for (const auto& handle : this->getHandles())
	{
		handle.shapePtr->setTranslation(pos.x, pos.y, pos.z);
		ShapeRenderer::draw(handle, ShapeRenderer::NO_DEPTH_TEST);
	}
}

std::optional<Vec3> Gizmo::getIntersectingHandleAxis(const Ray& ray) const
{
	std::optional<Vec3> axis;
	const auto& handles = getHandles();

	if (handles.at(0).shapePtr->isRayIntersecting(ray)) return axis.emplace(1, 0, 0);
	if (handles.at(1).shapePtr->isRayIntersecting(ray)) return axis.emplace(0, 1, 0);
	if (handles.at(2).shapePtr->isRayIntersecting(ray)) return axis.emplace(0, 0, 1);

	return axis;
}

// Scaling gizmo
constexpr float SCALE_GIZMO_HANDLE_SIZE = 0.08f;

const std::array<Object, 3> ScaleGizmo::s_Handles =
{
	Object(std::make_unique<Cube>(-1, 0, 0, SCALE_GIZMO_HANDLE_SIZE), Colours::RED),
	Object(std::make_unique<Cube>(0, -1, 0, SCALE_GIZMO_HANDLE_SIZE), Colours::GREEN),
	Object(std::make_unique<Cube>(0, 0, 1, SCALE_GIZMO_HANDLE_SIZE), Colours::BLUE)
};

std::function<void(const Object&)> ScaleGizmo::getTransformation(const Vec3& axis, float magnitude) const
{
	return { [&axis, magnitude](const Object& obj)
	{
		const auto scale = axis * magnitude;
		obj.shapePtr->addScale(scale.x, scale.y, scale.z);
	}};
}

const std::array<Object, 3>& ScaleGizmo::getHandles() const
{
	return s_Handles;
}

// Translating gizmo
constexpr float TRANSLATE_GIZMO_HANDLE_SIZE = 0.08f;

// TODO cone
const std::array<Object, 3> TranslateGizmo::s_Handles =
{
	Object(std::make_unique<Cube>(-1, 0, 0, TRANSLATE_GIZMO_HANDLE_SIZE), Colours::RED),
	Object(std::make_unique<Cube>(0, -1, 0, TRANSLATE_GIZMO_HANDLE_SIZE), Colours::GREEN),
	Object(std::make_unique<Cube>(0, 0, 1, TRANSLATE_GIZMO_HANDLE_SIZE), Colours::BLUE)
};

std::function<void(const Object&)> TranslateGizmo::getTransformation(const Vec3& axis, float magnitude) const
{
	return { [&axis, magnitude](const Object& obj)
	{
		const auto translation = axis * magnitude;
		obj.shapePtr->addTranslation(translation.x, translation.y, translation.z);
	}};
}

const std::array<Object, 3>& TranslateGizmo::getHandles() const
{
	return s_Handles;
}

// Rotating gizmo
constexpr float ROTATE_GIZMO_HANDLE_SIZE = 0.08f;

// TODO help
const std::array<Object, 3> RotateGizmo::s_Handles =
{
	Object(std::make_unique<Cube>(-1, 0, 0, ROTATE_GIZMO_HANDLE_SIZE), Colours::RED),
	Object(std::make_unique<Cube>(0, -1, 0, ROTATE_GIZMO_HANDLE_SIZE), Colours::GREEN),
	Object(std::make_unique<Cube>(0, 0, 1, ROTATE_GIZMO_HANDLE_SIZE), Colours::BLUE)
};

std::function<void(const Object&)> RotateGizmo::getTransformation(const Vec3& axis, float magnitude) const
{
	return { [&axis, magnitude](const Object& obj)
	{
		const auto rotation = axis * magnitude;
		obj.shapePtr->addRotation(rotation.x, rotation.y, rotation.z);
	}};
}

const std::array<Object, 3>& RotateGizmo::getHandles() const
{
	return s_Handles;
}