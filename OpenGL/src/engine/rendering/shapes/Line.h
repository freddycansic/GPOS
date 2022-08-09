#pragma once
#include "Shape.h"

class Line : public Shape
{
	const static std::vector<unsigned int> s_Indices;
	const static std::vector<Vec3> s_UnitPositions;
	const static std::vector<Vec2> s_TextureCoordinates;
	
public:
	Line(float x1, float y1, float z1, float x2, float y2, float z2);
	Line(const Vec3& p1, const Vec3& p2);
	Line() = default;

	[[nodiscard]] const std::vector<Vec3>& getUnitPositions() const override;
	[[nodiscard]] const std::vector<Vec2>& getTextureCoordinates() const override;
	[[nodiscard]] const std::vector<unsigned int>& getIndices() const override;
};

