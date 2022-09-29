#pragma once
#include "Shape.h"
#include "maths/Maths.h"

class Line : public Shape
{
	static Mesh s_Mesh;
	Vec3 m_P1, m_P2;

public:
	Line(float x1, float y1, float z1, float x2, float y2, float z2, float width);
	Line(const Vec3& p1, const Vec3& p2, float width);
	Line() = default;
	
	[[nodiscard]] Mesh& getMesh() const override;
	[[nodiscard]] Mat4x4 getTransformMatrix() const override;

	void setP1(float x, float y, float z);
	void setP1(const Vec3& pos);
	[[nodiscard]] const Vec3& getP1() const;

	void setP2(float x, float y, float z);
	void setP2(const Vec3& pos);
	[[nodiscard]] const Vec3& getP2() const;

};

