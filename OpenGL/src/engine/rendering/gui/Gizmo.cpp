#include "Gizmo.h"

#include "engine/Colours.h"
#include "engine/rendering/ObjectRenderer.h"
#include "engine/rendering/object/shapes/Line.h"
#include "engine/rendering/object/shapes/Cube.h"

constexpr float GIZMO_LINE_WIDTH = 0.02f;

std::array<Line, 3> Gizmo::s_Lines =
{
	Line(0, 0, 0, -1, 0, 0, GIZMO_LINE_WIDTH, Colours::RED),
	Line(0, 0, 0, 0, -1, 0, GIZMO_LINE_WIDTH, Colours::GREEN),
	Line(0, 0, 0, 0, 0, 1, GIZMO_LINE_WIDTH, Colours::BLUE)
};

void Gizmo::render(const Vec3& pos) const
{
	for (auto& line : s_Lines)
	{
		line.setTranslation(pos.x, pos.y, pos.z);
		ShapeRenderer::draw(line, ShapeRenderer::NO_DEPTH_TEST);
	}

	for (auto& handle : this->getHandles())
	{
		handle->setTranslation(pos.x, pos.y, pos.z);
		ShapeRenderer::draw(*handle, ShapeRenderer::NO_DEPTH_TEST);
	}
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

// Scaling gizmo
constexpr float SCALE_GIZMO_HANDLE_SIZE = 0.08f;

std::array<std::unique_ptr<Object>, 3> ScaleGizmo::s_Handles =
{
	std::make_unique<Cube>(-1, 0, 0, SCALE_GIZMO_HANDLE_SIZE, Colours::RED),
	std::make_unique<Cube>(0, -1, 0, SCALE_GIZMO_HANDLE_SIZE, Colours::GREEN),
	std::make_unique<Cube>(0, 0, 1, SCALE_GIZMO_HANDLE_SIZE, Colours::BLUE)
};

std::function<void(Object&)> ScaleGizmo::getTransformation(const Vec3& axis, float magnitude) const
{
	return { [&axis, magnitude](Object& obj)
	{
		const auto scale = axis * magnitude;
		obj.setScale(scale.x, scale.y, scale.z);
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

std::function<void(Object&)> TranslateGizmo::getTransformation(const Vec3& axis, float magnitude) const
{
	return { [&axis, magnitude](Object& obj)
	{
		const auto translation = axis * magnitude;
		obj.setTranslation(translation.x, translation.y, translation.z);
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

std::function<void(Object&)> RotateGizmo::getTransformation(const Vec3& axis, float magnitude) const
{
	return { [&axis, magnitude](Object& obj)
	{
		const auto rotation = axis * magnitude;
		obj.setRotation(rotation.x, rotation.y, rotation.z);
	}};
}

const std::array<std::unique_ptr<Object>, 3>& RotateGizmo::getHandles() const
{
	return s_Handles;
}