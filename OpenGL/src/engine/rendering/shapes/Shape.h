#pragma once

#include "Mesh.h"
#include "maths/Transform.h"

struct Vec3;
struct Vec2;

class Shape
{
protected:
	Transform m_Transform;
	std::vector<Vec3> m_Positions;
	bool m_Moved = true;

public:
	virtual ~Shape() = default;

	virtual void setScale(float xScale, float yScale, float zScale);
	virtual void setRotation(float xRotation, float yRotation, float zRotation);
	virtual void setTranslation(float xTranslate, float yTranslate, float zTranslate);

	void setMoved(bool moved);
	[[nodiscard]] bool moved() const;

	void setPositions(const std::vector<Vec3>& positions);
	[[nodiscard]] std::vector<Vec3> getPositions() const;

	[[nodiscard]] virtual Mesh& getMesh() const = 0; // should to point to static Mesh in child class
	[[nodiscard]] virtual Mat4 getTransformMatrix() const;

	// TODO TEMPORARY
	float* scalePtr();
	float* rotationPtr();
	float* translationPtr();
};