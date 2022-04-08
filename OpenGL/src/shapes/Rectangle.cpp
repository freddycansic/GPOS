#include "Rectangle.h"

#include <vector>

#include "maths/Mat4.h"

const std::array<Vertex, 4> Rectangle::s_UnitVertices =
{
	Vertex({ -0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, -1.0f),
	Vertex({ -0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, -1.0f),
	Vertex({  0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, -1.0f),
	Vertex({  0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, -1.0f)
};

const std::array<unsigned int, 6> Rectangle::s_UnitIndices =
{
	0, 1, 2,
	0, 2, 3,
};

Rectangle::Rectangle(float x, float y, float width, float height)
: Shape(x, y), m_Width(width), m_Height(height) {
	vertices.reserve(4);

	for (const auto& unitVertex : s_UnitVertices) {
		// copy unit vertex
		Vertex newVertex = unitVertex;

		// set new position
		newVertex.position = Vec3(Mat4::identity.translate(x, y, 0.0f).rotate(0, 0, 0).scale(width, height, 1.0f) * Vec4(unitVertex.position, 1.0f));

		// add new vertex to the buffer TODO TEMPORARY
		vertices.push_back(newVertex);

		m_Vertices.push_back(newVertex);
	}

}


