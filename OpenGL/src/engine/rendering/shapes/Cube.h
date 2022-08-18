#pragma once

#include "Shape.h"

class Cube : public Shape
{
private:
	const static std::vector<unsigned int> s_Indices;
	const static std::vector<Vec3> s_UnitPositions;
	const static std::vector<Vec2> s_TextureCoordinates;

	float m_Size = 0.0f;

public:
	Cube(float x, float y, float z, float size);
	Cube(const Vec3& pos, float size);
	Cube() = default;

	[[nodiscard]] const std::vector<Vec3>& getUnitPositions() const override;
	[[nodiscard]] const std::vector<Vec2>& getTextureCoordinates() const override;
	[[nodiscard]] const std::vector<unsigned int>& getIndices() const override;

	void setScale(float xScale, float yScale, float zScale) override;
};


