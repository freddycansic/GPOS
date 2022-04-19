#pragma once

#include <vector>

#include "engine/rendering/Vertex.h"

class Shape
{
protected:
	float m_XPos, m_YPos, m_ZPos;

private:
	Vec3 m_Rotation;
	std::vector<Vertex> vertices;

public:
	Shape(float x, float y, float z) 
	: m_XPos(x), m_YPos(y), m_ZPos(z)
	{}

	virtual std::vector<Vertex> getVertices() const = 0;
	virtual std::vector<unsigned int> getIndices() const = 0;

	void setRotation(float xRotation, float yRotation, float zRotation);
};

