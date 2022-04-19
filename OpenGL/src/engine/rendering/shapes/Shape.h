#pragma once

#include <vector>

#include "engine/rendering/Vertex.h"

class Shape
{
private:
	Vec3 m_Rotation, m_Translation, m_Scale = {1.0f, 1.0f, 1.0f};

protected:
	float m_XPos, m_YPos, m_ZPos;
	std::vector<Vertex> m_Vertices;

	void recalculateVertices();

public:
	Shape(float x, float y, float z)
		: m_XPos(x), m_YPos(y), m_ZPos(z)
	{}

	virtual std::vector<Vertex> getVertices() const = 0;
	virtual std::vector<unsigned int> getIndices() const = 0;

	void setRotation(float xRotation, float yRotation, float zRotation);
	void setScale(float xScale, float yScale, float zScale);
	void setTranslate(float xTranslate, float yTranslate, float zTranslate);
};
