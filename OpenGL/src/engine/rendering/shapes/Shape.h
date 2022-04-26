#pragma once

#include <vector>

#include "engine/rendering/Vertex.h"
#include "maths/Transform.h"

class Shape
{
protected:
	Transform m_Transform;
	std::vector<Vertex> m_Vertices;

public:
	Shape() = default;

	// define these when creating a shape
	virtual std::vector<Vertex> getUnitVertices() const = 0;
	virtual std::vector<Vertex> getVertices() = 0;
	virtual std::vector<unsigned int> getIndices() const = 0;

	void recalculateVertices();
	virtual void setScale(float xScale, float yScale, float zScale); // all shapes will scale differently based off of original shape
	virtual void setRotation(float xRotation, float yRotation, float zRotation);
	virtual void setTranslation(float xTranslate, float yTranslate, float zTranslate);
};