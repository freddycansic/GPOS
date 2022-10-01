#pragma once

#include "Object.h"

class Cube : public Object
{
private:
	static Mesh s_Mesh;
	float m_Size = 0.0f;

public:
	Cube(float x, float y, float z, float size);
	Cube(const Vec3& pos, float size);
	Cube() = default;

	[[nodiscard]] Mesh& getMesh() const override;

	void setScale(float xScale, float yScale, float zScale) override;
};


