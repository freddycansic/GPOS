#pragma once

#include "Shape.h"

class Cube : public Shape
{
private:
	const static std::vector<Vertex> s_UnitVertices;
	const static std::vector<unsigned int> s_UnitIndices;

	float m_Size = 0.0f;

public:
	Cube(float x, float y, float z, float size);
	Cube() = default;

	std::vector<Vertex> getVertices() const override;
	std::vector<unsigned int> getIndices() const override;
};


