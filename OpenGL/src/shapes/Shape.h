#pragma once

#include <vector>

#include "engine/Vertex.h"

class Shape
{
protected:
	float m_XPos, m_YPos;

private:

public:
	Shape(float x, float y) 
	: m_XPos(x), m_YPos(y)
	{}

	void setTexID(unsigned int ID); // TODO TEMPORARY
	std::vector<Vertex> vertices; // TODO TEMPORARY
	
};

