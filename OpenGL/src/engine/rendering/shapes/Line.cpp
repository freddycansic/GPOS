#include "Line.h"

const std::vector<unsigned int> Line::s_UnitIndices;
const std::vector<Vertex> Line::s_UnitVertices;

Line::Line(float x1, float y1, float z1, float x2, float y2, float z2, float width)
{
	const float m = (y2 - y1) / (x2 - x1);
	//const float yIntercept = (y1 - x1 * gradient);

	const float nm = -1 / m; // normal gradient to line between p1 and p2
	const float p1c = (y1 - x1 * nm); // y intercept of line at p1



}

Line::Line(const Vec3& pos1, const Vec3& pos2, float size) : Line(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z, size)
{
}

void Line::recalculateVertices()
{
}

std::vector<Vertex>& Line::getVertices()
{
	return m_Vertices;
}

const std::vector<Vertex>& Line::getUnitVertices() const
{
	return m_Vertices;
}

const std::vector<unsigned int>& Line::getIndices() const
{
	return s_UnitIndices;
}
