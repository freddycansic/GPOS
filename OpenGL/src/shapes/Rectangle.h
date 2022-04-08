#pragma once

#include <array>

#include "Shape.h"

class Rectangle : public Shape
{
private:
	const static std::array<Vertex, 4> s_UnitVertices;
	const static std::array<unsigned int, 6> s_UnitIndices;

	float m_Width, m_Height;

public:
	Rectangle(float x, float y, float width, float height);

};

