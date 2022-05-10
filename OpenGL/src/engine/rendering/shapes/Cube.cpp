#include "Cube.h"

const std::vector<Vertex> Cube::s_UnitVertices =
{
	Vertex({ -0.5f, -0.5f,  0.5f }, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, -1.0f), // 0 bottom front left
	Vertex({  0.5f, -0.5f,  0.5f }, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, -1.0f), // 1 bottom front right
	Vertex({  0.5f, -0.5f, -0.5f }, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, -1.0f), // 2 bottom back right
	Vertex({ -0.5f, -0.5f, -0.5f }, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, -1.0f), // 3 bottom back left
	Vertex({ -0.5f,  0.5f,  0.5f }, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, -1.0f), // 4 top front left
	Vertex({  0.5f,  0.5f,  0.5f }, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, -1.0f), // 5 top front right
	Vertex({  0.5f,  0.5f, -0.5f }, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, -1.0f), // 6 top back right
	Vertex({ -0.5f,  0.5f, -0.5f }, {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, -1.0f)  // 7 top back left
};

const std::vector<unsigned int> Cube::s_UnitIndices =
{
	0, 1, 2, // bottom face
	2, 3, 0,
	2, 6, 3, // back face
	3, 7, 6,
	3, 0, 4, // left face
	4, 7, 3,
	0, 4, 5, // front face
	5, 0, 1,
	1, 2, 5, // right face
	5, 6, 2,
	4, 5, 6, // top face
	6, 7, 4
};

Cube::Cube(float x, float y, float z, float size)
	: m_Size(size)
{
	m_Transform.tra = { x, y, z };
	m_Transform.rot = { 0.0f, 0.0f, 0.0f };
	m_Transform.sca = { size, size, size };

	recalculateVertices();
}

void Cube::setScale(float xScale, float yScale, float zScale) {
	m_Transform.sca = { m_Size * xScale, m_Size * yScale, m_Size * zScale };
}

const std::vector<Vertex>& Cube::getUnitVertices() const {
	return s_UnitVertices;
}

std::vector<Vertex>& Cube::getVertices() 
{
	return m_Vertices;
}

const std::vector<unsigned int>& Cube::getIndices() const
{
	return s_UnitIndices;
}