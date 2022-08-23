#pragma once

#include "Mesh.h"
#include "maths/Transform.h"

struct Vec3;
struct Vec2;

class Shape
{
protected:
	Transform m_Transform;

public:
	virtual ~Shape() = default;

	[[nodiscard]] virtual const Mesh& getMesh() const = 0; // should to point to static Mesh in child class
	[[nodiscard]] virtual Mat4 getTransformMatrix() const;

	virtual void setScale(float xScale, float yScale, float zScale);
	virtual void setRotation(float xRotation, float yRotation, float zRotation);
	virtual void setTranslation(float xTranslate, float yTranslate, float zTranslate);

	// TODO TEMPORARY
	float* scalePtr();
	float* rotationPtr();
	float* translationPtr();
};