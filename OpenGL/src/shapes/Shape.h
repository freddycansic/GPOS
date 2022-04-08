#pragma once

#include <vector>

#include "engine/Vertex.h"

class Shape
{
protected:
	float m_XPos, m_YPos;
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	
private:

public:
	Shape(float x, float y) 
	: m_XPos(x), m_YPos(y)
	{}

	std::vector<Vertex> getVertices() const { return m_Vertices; }
	std::vector<unsigned int> getIndices() const { return m_Indices; }

	void setTexID(unsigned int ID); // TODO TEMPORARY
	std::vector<Vertex> vertices; // TODO TEMPORARY
};

