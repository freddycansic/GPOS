#include "Circle.h"

Mesh Circle::s_Mesh =
{
	{
		{0, 0, 0},
		{0, 1, 0},
		{0.587785f, 0.809017f, 0},
		{0.951057f, 0.309017f, 0},
		{0.951056f, -0.309017f, 0},
		{0.587785f, -0.809017f, 0},
		{0, -1, 0},
		{-0.587785f, -0.809017f, 0},
		{-0.951056f, -0.309017f, 0},
		{-0.951057f, 0.309017f, 0},
		{-0.587786f, 0.809017f, 0}
	},

	{
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0},
		{0, 0}
	},

	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 8,
		0, 9, 10,
		0, 10, 1
	}
};

Circle::Circle(float centerX, float centerY, float radius)
{
	m_Transform.tra = { centerX, centerY, 0 };
	m_Transform.sca = { radius, radius, radius };
}

Circle::Circle(const Vec2& center, float radius) :
	Circle(center.x, center.y, radius)
{
}

Mesh& Circle::getMesh() const
{
	return s_Mesh;
}
