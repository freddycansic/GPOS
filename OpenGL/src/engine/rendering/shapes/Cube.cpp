#include "Cube.h"

#include "maths/Mat4.h"

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
	: Shape(x, y, z), m_Size(size)
{
	std::cout << "CTOR" << std::endl;
	m_Vertices.reserve(s_UnitVertices.size());
	
	// copy vertices
	m_Vertices = s_UnitVertices;

	// apply scale, rotate + translation to vertices
	recalculateVertices();
	std::cout << "CTOR STOP" << std::endl;
}

std::vector<Vertex> Cube::getVertices() const
{
	return m_Vertices;
}

std::vector<unsigned int> Cube::getIndices() const
{
	return s_UnitIndices;
}
