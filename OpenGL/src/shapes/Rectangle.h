#pragma once

#include <array>

#include "Shape.h"

class Rectangle : public Shape
{
private:
	const static std::vector<Vertex> s_UnitVertices;
	const static std::vector<unsigned int> s_UnitIndices;

	// no need for indices field as they never change
	std::vector<Vertex> m_Vertices;

	float m_Width, m_Height;

public:
	Rectangle(float x, float y, float width, float height);

	std::vector<Vertex> getVertices() const override;
	std::vector<unsigned int> getIndices() const override;
};

