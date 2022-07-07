#include "Line.h"

#include "Cube.h"
#include "maths/Maths.h"

Line::Line(float x1, float y1, float z1, float x2, float y2, float z2, float width) : m_Width(width)
{
	const float dx = x1 - x2;
	const float dy = y1 - y2;
	const float dz = z1 - z2;

	const float magxz = std::sqrt(std::pow(dx, 2) + std::pow(dz, 2));
	const float magxy = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
	const float mag = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2) + std::pow(dz, 2));

	const float pitch = std::asin(dy / magxy);
	const float yaw = std::asin(dz / magxz);

	Cube c(0, 0, 0, width / 2);
	c.setScale(mag, 1, 1);
	c.setRotation(0, -Maths::degrees(yaw), Maths::degrees(pitch));
	c.recalculateVertices();

	m_Vertices = c.getVertices(); // TODO getVertices returns reference = could go wrong when out of scope
}

Line::Line(const Vec3& pos1, const Vec3& pos2, float width) : Line(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z, width)
{
}

void Line::recalculateVertices()
{
}

std::vector<Vertex>& Line::getVertices()
{
	return m_Vertices;
}