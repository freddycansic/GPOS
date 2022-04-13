#pragma once

#include <vector>

#include "engine/rendering/Vertex.h"

class Shape
{
protected:
	float m_XPos, m_YPos;
	
private:


public:
	Shape(float x, float y) 
	: m_XPos(x), m_YPos(y)
	{}

	virtual std::vector<Vertex> getVertices() const = 0;
	virtual std::vector<unsigned int> getIndices() const = 0;
};

