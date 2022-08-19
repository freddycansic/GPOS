#include "Line.h"

#include "maths/Maths.h"

constexpr float PI_OVER_3 = static_cast<float>(Maths::PI) / 3.0f;

const std::vector<Vec3> Line::s_UnitPositions = 
{
	{0, sinf(0 * PI_OVER_3), cosf(0 * PI_OVER_3)},
	{0, sinf(2 * PI_OVER_3), cosf(2 * PI_OVER_3)},
	{0, sinf(4 * PI_OVER_3), cosf(4 * PI_OVER_3)},

	{1, sinf(0 * PI_OVER_3), cosf(0 * PI_OVER_3)},
	{1, sinf(2 * PI_OVER_3), cosf(2 * PI_OVER_3)},
	{1, sinf(4 * PI_OVER_3), cosf(4 * PI_OVER_3)},
};

const std::vector<unsigned int> Line::s_Indices =
{
	0, 1, 2,

	0, 3, 1,
	3, 1, 4,

	1, 4, 2,
	4, 2, 5,

	2, 5, 3,
	5, 3, 0,

	3, 4, 5,
};

const std::vector<Vec2> Line::s_TextureCoordinates =
{
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 0.0f},
};

Line::Line(float x1, float y1, float z1, float x2, float y2, float z2, float width)
{
	m_Transform.sca = { 1, width, width };
	m_P1 = { x1, y1, z1 };
	m_P2 = { x2, y2, z2 };
}

Line::Line(const Vec3& p1, const Vec3& p2, float width) : Line(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, width)
{
}

const Vec3 i = { 1.0f, 0.0f, 0.0f };

void Line::recalculatePositions()
{
	if (m_Positions.empty()) {
		m_Positions = getUnitPositions();
	}

	if (!m_Moved) return;

	const Vec3 diff = m_P1 - m_P2; // difference between the two points
	const float mag = diff.magnitude(); // mag of diff

	Vec3 axis = (i.cross(diff)).normalise();

	if (std::isnan(axis.x)) axis = { 0, 0, 0 };

	const float theta = i.angleBetween(diff);

	const Mat4 transformMatrix =
		Mat4::identity.translate(
			m_P2.x,
			m_P2.y,
			m_P2.z
		).rotateAxis(
			axis.x,
			axis.y,
			axis.z,
			Maths::degrees(theta)
		).scale(
			mag,
			m_Transform.sca.y,
			m_Transform.sca.z
		);

	const auto& unitVertices = getUnitPositions();

	for (unsigned int i = 0; i < m_Positions.size(); i++) {
		const auto& unitPos = unitVertices.at(i); // unit pos
		auto& resultPos = m_Positions.at(i); // result vertex

		// apply transformation to unit vertex and store in result vertex
		resultPos = Vec3(Vec4(unitPos, 1.0f) * transformMatrix);
	}

	m_Moved = false;
}

void Line::setP1(float x, float y, float z)
{
	m_P1 = { x, y, z };
	m_Moved = true;
}

void Line::setP1(const Vec3& pos)
{
	m_P1 = pos;
	m_Moved = true;
}

const Vec3& Line::getP1() const
{
	return m_P1;
}

void Line::setP2(float x, float y, float z)
{
	m_P2 = { x, y, z };
	m_Moved = true;
}

void Line::setP2(const Vec3& pos)
{
	m_P2 = pos;
	m_Moved = true;
}

const Vec3& Line::getP2() const
{
	return m_P2;
}

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
