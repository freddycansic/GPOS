#pragma once

#include "Shape.h"

class Cube : public Shape
{
private:
	const static Mesh s_Mesh;
	float m_Size = 0.0f;

public:
	Cube(float x, float y, float z, float size);
	Cube(const Vec3& pos, float size);
	Cube() = default;

	[[nodiscard]] const Mesh& getMesh() const override;

	void setScale(float xScale, float yScale, float zScale) override;
};


