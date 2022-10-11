#pragma once

#include <optional>

#include "Mesh.h"
#include "maths/Maths.h"
#include "maths/Transform.h"

class Cube;
struct Vec3;
struct Vec2;
struct Ray;

class Shape
{
protected:
	Transform m_Transform;
	std::vector<Vec3> m_Positions;
	bool m_Moved = true;
	bool m_Selectable = true;

public:
	virtual ~Shape() = default;

	virtual void setScale(float xScale, float yScale, float zScale);
	virtual void setRotation(float xRotation, float yRotation, float zRotation);
	virtual void setTranslation(float xTranslate, float yTranslate, float zTranslate);

	[[nodiscard]] Cube getAABB() const;

	[[nodiscard]] std::optional<Vec4> isRayIntersecting(const Ray& ray) const;

	void setMoved(bool moved);
	[[nodiscard]] bool moved() const;

	void setPositions(const std::vector<Vec3>& positions);
	[[nodiscard]] const std::vector<Vec3>& getPositions() const;

	[[nodiscard]] virtual Mesh& getMesh() const = 0; // should to point to static Mesh in child class
	[[nodiscard]] virtual Mat4x4 getTransformMatrix() const;
};