#pragma once

#include <vector>

#include "engine/rendering/Vertex.h"
#include "maths/Transform.h"

class Shape
{
protected:
	Transform m_Transform;
	float m_XPos = 0.0f, m_YPos = 0.0f, m_ZPos = 0.0f;
	std::vector<Vertex> m_Vertices;

public:
	Shape(float x, float y, float z)
		: m_XPos(x), m_YPos(y), m_ZPos(z)
	{}

	Shape() = default;

	// define these when creating a shape
	virtual void recalculateVertices() = 0;
	[[nodiscard]] virtual std::vector<Vertex> getVertices() const = 0;
	virtual std::vector<unsigned int> getIndices() const = 0;

	virtual void setRotation(float xRotation, float yRotation, float zRotation);
	virtual void setScale(float xScale, float yScale, float zScale);
	virtual void setTranslation(float xTranslate, float yTranslate, float zTranslate);
};
