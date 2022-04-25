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

	/**
	Define in child class as applying transformation to vertex from unit vertices and add to member vertices
	**/

public:
	Shape(float x, float y, float z)
		: m_XPos(x), m_YPos(y), m_ZPos(z)
	{}

	Shape() = default;

	virtual void recalculateVertices() = 0;
	virtual std::vector<Vertex> getVertices() = 0;
	virtual std::vector<unsigned int> getIndices() const = 0;

	virtual void setScale(float xScale, float yScale, float zScale) = 0; // all shapes will scale differently based off of original shape
	virtual void setRotation(float xRotation, float yRotation, float zRotation);
	virtual void setTranslation(float xTranslate, float yTranslate, float zTranslate);
};
