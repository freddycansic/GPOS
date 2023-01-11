#pragma once

#include <array>
#include <optional>
#include <memory>

#include "engine/rendering/objects/Line.h"
#include "engine/rendering/objects/Object.h"
#include "maths/Vec3.h"

enum class GizmoTool
{
	TRANSLATE,
	ROTATE,
	SCALE
};

namespace Gizmo
{
	void render(const Vec3& pos);

	[[nodiscard]] std::optional<Vec3> getIntersectingHandleAxis(const Ray& ray);
	[[nodiscard]] std::optional<Vec3> getHandleIntersectionPoint(const Ray& ray);

	void transformObject(Object& object, const Vec3& transformation);

	void setTool(GizmoTool tool);
	[[nodiscard]] GizmoTool getTool();
};