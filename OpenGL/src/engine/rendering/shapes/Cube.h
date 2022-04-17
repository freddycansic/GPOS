#pragma once

#include "Shape.h"

class Cube : public Shape
{
private:
	const static std::vector<Vertex> s_UnitVertices;
	const static std::vector<unsigned int> s_UnitIndices;

	std::vector<Vertex> m_Vertices;

	float m_Size;

public:
	Cube(float x, float y, float z, float size);

	std::vector<Vertex> getVertices() const override;
	std::vector<unsigned int> getIndices() const override;
};


