#pragma once

#include "Shape.h"
#include "maths/Mat4.h"

class Cube : public Shape
{
private:
	const static std::vector<unsigned int> s_UnitIndices;
	const static std::vector<Vertex> s_UnitVertices;

	Mat4 transform = Mat4::identity;

	float m_Size = 0.0f;

	void recalculateVertices() override;

public:
	Cube(float x, float y, float z, float size);
	Cube() = default;

	std::vector<Vertex> getVertices() const override;
	std::vector<unsigned int> getIndices() const override;
	void setScale(float xScale, float yScale, float zScale) override;

	// TODO TEMPORARY
	void setTransform(const Mat4& mat);
};


