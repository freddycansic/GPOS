#pragma once

#include <vector>
#include <memory>

#include "maths/Vectors.h"

class Object;

struct Gizmo
{
	Vec3 pos;
	std::vector<std::pair<std::unique_ptr<Object>, Vec4>> shapes;

	Vec3 getClickedHandle() const;
};

struct ScaleGizmo : public Gizmo
{
	ScaleGizmo(float x, float y, float z);
	ScaleGizmo(const Vec3& pos);
};

struct TranslateGizmo : public Gizmo
{
	TranslateGizmo(float x, float y, float z);
	TranslateGizmo(const Vec3& pos);
};

struct RotateGizmo : public Gizmo
{
	RotateGizmo(float x, float y, float z);
	RotateGizmo(const Vec3& pos);
};