#include "Rectangle.h"

#include "maths/Mat4.h"

const std::vector<Vertex> Rectangle::s_UnitVertices =
{
	Vertex({ -0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}),
	Vertex({ -0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}),
	Vertex({  0.5f,  0.5f, 0.0f }, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}),
	Vertex({  0.5f, -0.5f, 0.0f }, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f})
};

const std::vector<unsigned int> Rectangle::s_UnitIndices =
{
	0, 1, 2,
	0, 2, 3,
};

Rectangle::Rectangle(float x, float y, float width, float height)
: m_Width(width), m_Height(height){
	m_Transform.tra = { x, y, 0.0f };
	m_Transform.rot = { 0.0f, 0.0f, 0.0f };
	m_Transform.sca = { width, height, 1.0f };

	recalculateVertices();
}

void Rectangle::setScale(float xScale, float yScale, float zScale) {
	m_Transform.sca = { m_Width * xScale, m_Height * yScale, 1.0f };
}

const std::vector<Vertex>& Rectangle::getUnitVertices() const {
	return s_UnitVertices;
}

std::vector<Vertex>& Rectangle::getVertices() {
	return m_Vertices;
}

const std::vector<unsigned int>& Rectangle::getIndices() const {
	return s_UnitIndices;
}


