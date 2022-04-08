#pragma once

#include <array>

#include "Shape.h"

class Rectangle : public Shape
{
private:
	const static std::array<Vertex, 4> s_UnitRectVertices;

	float m_Width, m_Height;

public:
	Rectangle(float x, float y, float width, float height);

};

