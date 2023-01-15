#pragma once

#include "Object.h"
#include "maths/Maths.h"

struct Line : public Object
{
public:
	Line(float x1, float y1, float z1, float x2, float y2, float z2, float width, const Material& material);
	Line(const Vec3& p1, const Vec3& p2, float width, const Material& material);
	Line(const Ray& ray, float length, float width, const Material& material);
	~Line() override = default;

	[[nodiscard]] const std::string& stringName() const override;

	[[nodiscard]] Mesh& getMesh() const override;
	[[nodiscard]] Mat4x4 getTransformMatrix() const override;

	Vec3 p1, p2;

private:
	static Mesh s_Mesh;
};

