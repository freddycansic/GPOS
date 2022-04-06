#pragma once

#include <vector>

#include "engine/Vertex.h"
#include "maths/Vectors.h"

class Shape
{
protected:
	float m_XPos, m_YPos;

private:

public:
	Shape(float x, float y) 
	: m_XPos(x), m_YPos(y)
	{}

	std::vector<Vertex> vertices; // TODO TEMPORARY
	
};

