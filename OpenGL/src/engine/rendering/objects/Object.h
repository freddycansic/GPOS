#pragma once

#include "Mesh.h"
#include "maths/Maths.h"
#include "maths/Transform.h"

class Cube;
struct Vec3;
struct Vec2;

class Object
{
protected:
	Transform m_Transform;
	std::vector<Vec3> m_Positions;
	bool m_Moved = true;
	bool m_Selected = false;
	bool m_Selectable = true;

public:
	virtual ~Object() = default;

	virtual void setScale(float xScale, float yScale, float zScale);
	virtual void setRotation(float xRotation, float yRotation, float zRotation);
	virtual void setTranslation(float xTranslate, float yTranslate, float zTranslate);

	void setMoved(bool moved);
	[[nodiscard]] bool moved() const;

	void setSelected(bool selected);
	[[nodiscard]] bool selected() const;

	void setSelectable(bool selectable);
	[[nodiscard]] bool selectable() const;

	void setPositions(const std::vector<Vec3>& positions);
	[[nodiscard]] std::vector<Vec3> getPositions() const;

	[[nodiscard]] Cube getAABB() const;

	[[nodiscard]] virtual Mesh& getMesh() const = 0; // should to point to static Mesh in child class
	[[nodiscard]] virtual Mat4x4 getTransformMatrix() const;
};