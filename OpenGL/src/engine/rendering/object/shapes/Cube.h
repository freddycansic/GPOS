#pragma once

#include "Object.h"

struct Cube : public Object
{
public:
	Cube(float x, float y, float z, float size, const Material& material);
	Cube(const Vec3& pos, float size, const Material& material);
	Cube() = default;
	~Cube() override = default;

	[[nodiscard]] Mesh& getMesh() const override;

	void setScale(float xScale, float yScale, float zScale) override;

private:
	static Mesh s_Mesh;
	float m_Size = 1.0f;

};


