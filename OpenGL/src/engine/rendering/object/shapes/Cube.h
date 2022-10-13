#pragma once

#include "Shape.h"

class Cube : public Shape
{
private:
	static Mesh s_Mesh;
	float m_Size = 1.0f;

public:
	Cube(float x, float y, float z, float size);
	Cube(const Vec3& pos, float size);
	Cube();
	~Cube() override = default;

	[[nodiscard]] Mesh& getMesh() const override;

	void setScale(float xScale, float yScale, float zScale) override;
};


