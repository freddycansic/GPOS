#pragma once
#include "Shape.h"

class Line : public Shape
{
	const static std::vector<unsigned int> s_Indices;
	const static std::vector<Vec3> s_UnitPositions;
	const static std::vector<Vec2> s_TextureCoordinates;

	Vec3 m_P1, m_P2;

public:
	Line(float x1, float y1, float z1, float x2, float y2, float z2, float width);
	Line(const Vec3& p1, const Vec3& p2, float width);
	Line() = default;

	void recalculatePositions() override;

	void setP1(float x, float y, float z);
	void setP1(const Vec3& pos);
	[[nodiscard]] const Vec3& getP1() const;

	void setP2(float x, float y, float z);
	void setP2(const Vec3& pos);
	[[nodiscard]] const Vec3& getP2() const;

	[[nodiscard]] const std::vector<Vec3>& getUnitPositions() const override;
	[[nodiscard]] const std::vector<Vec2>& getTextureCoordinates() const override;
	[[nodiscard]] const std::vector<unsigned int>& getIndices() const override;
};

