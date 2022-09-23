#pragma once
#include "Shape.h"

class Circle : public Shape
{
	static Mesh s_Mesh;
	float m_Radius = 0.0f;

public:
	Circle(float centerX, float centerY, float radius);
	Circle(const Vec2& center, float radius);
	Circle() = default;

	[[nodiscard]] Mesh& getMesh() const override;
};

