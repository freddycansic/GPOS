#pragma once

#include "engine/rendering/object/shapes/Shape.h"

class Cone : public Shape
{
private:
	static Mesh s_Mesh;

public:
	Cone(float x, float y, float z, float radius, float height);
	Cone(const Vec3& pos, float radius, float height);
	Cone() = default;
	~Cone() override = default;

	[[nodiscard]] Mesh& getMesh() const override;
};

