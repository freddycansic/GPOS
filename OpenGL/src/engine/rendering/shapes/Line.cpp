#include "Line.h"

#include "maths/Maths.h"

constexpr float PI_OVER_5 = static_cast<float>(Maths::PI) / 5.0f;

const std::vector<Vec3> Line::s_UnitPositions = 
{
	{0, 0, -0.5f}, // FRONT FACE
	{sinf(0), cosf(0), -0.5f},
	{sinf(PI_OVER_5), cosf(PI_OVER_5), -0.5f},
	{sinf(2 * PI_OVER_5), cosf(2 * PI_OVER_5), -0.5f},
	{sinf(3 * PI_OVER_5), cosf(3 * PI_OVER_5), -0.5f},
	{sinf(4 * PI_OVER_5), cosf(4 * PI_OVER_5), -0.5f},
	{sinf(5 * PI_OVER_5), cosf(5 * PI_OVER_5), -0.5f},
	{sinf(6 * PI_OVER_5), cosf(6 * PI_OVER_5), -0.5f},
	{sinf(7 * PI_OVER_5), cosf(7 * PI_OVER_5), -0.5f},
	{sinf(8 * PI_OVER_5), cosf(8 * PI_OVER_5), -0.5f},
	{sinf(9 * PI_OVER_5), cosf(9 * PI_OVER_5), -0.5f},

	{0, 0, 0.5f},
	{sinf(0), cosf(0), 0.5f},
	{sinf(PI_OVER_5), cosf(PI_OVER_5), 0.5f},
	{sinf(2 * PI_OVER_5), cosf(2 * PI_OVER_5), 0.5f},
	{sinf(3 * PI_OVER_5), cosf(3 * PI_OVER_5), 0.5f},
	{sinf(4 * PI_OVER_5), cosf(4 * PI_OVER_5), 0.5f},
	{sinf(5 * PI_OVER_5), cosf(5 * PI_OVER_5), 0.5f},
	{sinf(6 * PI_OVER_5), cosf(6 * PI_OVER_5), 0.5f},
	{sinf(7 * PI_OVER_5), cosf(7 * PI_OVER_5), 0.5f},
	{sinf(8 * PI_OVER_5), cosf(8 * PI_OVER_5), 0.5f},
	{sinf(9 * PI_OVER_5), cosf(9 * PI_OVER_5), 0.5f},
};

const std::vector<unsigned int> Line::s_Indices =
{
	0, 1, 2, // FRONT FACE
	0, 2, 3,
	0, 3, 4,
	0, 4, 5,
	0, 5, 6,
	0, 6, 7,
	0, 7, 8,
	0, 8, 9,
	0, 9, 10,
	0, 10, 1,

	1, 12, 2, // MIDDLE
	12, 2, 13,
	2, 13, 3,
	13, 3, 14,
	3, 14, 4,
	14, 4, 15,
	4, 15, 5,
	15, 5, 16,
	5, 16, 6,
	16, 6, 17,
	6, 17, 7,
	17, 7, 18,
	7, 18, 8,
	18, 8, 19,
	8, 19, 9,
	19, 9, 20,
	9, 20, 10,
	20, 10, 21,
	10, 21, 1,
	21, 1, 12,

	11, 12, 13, // BACK FACE
	11, 13, 14,
	11, 14, 15,
	11, 15, 16,
	11, 16, 17,
	11, 17, 18,
	11, 18, 19,
	11, 19, 20,
	11, 20, 21,
	11, 21, 12,
};

const std::vector<Vec2> Line::s_TextureCoordinates =
{
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
};

const std::vector<Vec3>& Line::getUnitPositions() const
{
	return s_UnitPositions;
}

const std::vector<Vec2>& Line::getTextureCoordinates() const
{
	return s_TextureCoordinates;
}

const std::vector<unsigned int>& Line::getIndices() const
{
	return s_Indices;
}
