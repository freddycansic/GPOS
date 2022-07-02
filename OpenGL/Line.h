#pragma once

#include "engine/rendering/shapes/Shape.h"

class Line : public Shape
{
private:
	const static std::vector<unsigned int> s_UnitIndices;
	const static std::vector<Vertex> s_UnitVertices;

	float m_Width = 1.0f;

public:
	Line(float x1, float y1, float z1, float x2, float y2, float z2, float width);
	Line(const Vec3& pos1, const Vec3&pos2, float size);
	Line() = default;

	std::vector<Vertex>& getVertices() override;
	const std::vector<Vertex>& getUnitVertices() const override;
	const std::vector<unsigned int>& getIndices() const override;
};

