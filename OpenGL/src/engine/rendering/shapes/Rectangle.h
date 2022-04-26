#pragma once

#include <vector>

#include "Shape.h"

class Rectangle : public Shape
{
private:
	const static std::vector<Vertex> s_UnitVertices;
	const static std::vector<unsigned int> s_UnitIndices;

	float m_Width = 0.0f, m_Height = 0.0f;

public:
	Rectangle(float x, float y, float width, float height);
	Rectangle() = default;

	void setScale(float xScale, float yScale, float zScale) override;
	std::vector<Vertex> getVertices() override;
	std::vector<unsigned int> getIndices() const override;
	std::vector<Vertex> getUnitVertices() const override;
};

