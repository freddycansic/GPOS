#include "Rectangle.h"

#include "maths/Mat4.h"

const std::vector<Vertex> Rectangle::s_UnitVertices =
{
	Vertex({ -0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, -1.0f),
	Vertex({ -0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, -1.0f),
	Vertex({  0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, -1.0f),
	Vertex({  0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, -1.0f)
};

const std::vector<unsigned int> Rectangle::s_UnitIndices =
{
	0, 1, 2,
	0, 2, 3,
};

Rectangle::Rectangle(float x, float y, float width, float height)
: Shape(x, y, 0.0f), m_Width(width), m_Height(height){
	m_Vertices.reserve(s_UnitVertices.size());

	for (const auto& unitVertex : s_UnitVertices) {
		// copy unit vertex
		Vertex newVertex = unitVertex;

		// set new position
		newVertex.position = Vec3(Mat4::identity.translate(x, y, 0.0f).rotate(0, 0, 0).scale(width, height, 1.0f) * Vec4(unitVertex.position, 1.0f));

		// add vertex to the buffer
		m_Vertices.push_back(newVertex);
	}
}

void Rectangle::setScale(float xScale, float yScale, float zScale) {
	m_Transform.sca = { m_Width * xScale, m_Height * yScale, 1.0f };
}

std::vector<Vertex> Rectangle::getVertices() {
	return m_Vertices;
}

std::vector<unsigned int> Rectangle::getIndices() const {
	return s_UnitIndices;
}


