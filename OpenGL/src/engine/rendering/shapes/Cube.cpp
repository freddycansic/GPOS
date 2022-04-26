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
	: m_Size(size)
{
	m_Transform.tra = { x, y, z };
	m_Transform.rot = { 0.0f, 0.0f, 0.0f };
	m_Transform.sca = { size, size, size };

	m_Vertices.reserve(s_UnitVertices.size());
	
	// copy vertices
	m_Vertices = s_UnitVertices;

	// apply scale, rotate + translation to vertices
	recalculateVertices();
}

void Cube::recalculateVertices()
{
	Mat4 transformMatrix = Mat4::identity.translate(m_Transform.tra.x, m_Transform.tra.y, m_Transform.tra.z).rotate(m_Transform.rot.x, m_Transform.rot.y, m_Transform.rot.z).scale(m_Transform.sca.x, m_Transform.sca.y, m_Transform.sca.z);
	
	//std::cout << m_Transform << std::endl;
	//std::cout << transformMatrix << std::endl;

	for (unsigned int i = 0; i < s_UnitVertices.size(); i++) {
		const auto& unitPos = s_UnitVertices[i].position; // unit pos
		auto& resultPos = m_Vertices[i].position; // result vertex
		//std::cout << "UNIT POS\n" << unitPos << std::endl;
		// apply transformation to unit vertex and store in result vertex
		resultPos = Vec3(Vec4(unitPos, 1.0f) * transformMatrix);
		//std::cout << "END POS\n" << resultPos << std::endl;
	}
}

void Cube::setScale(float xScale, float yScale, float zScale) {
	m_Transform.sca = { m_Size * xScale, m_Size * yScale, m_Size * zScale };
}

std::vector<Vertex> Cube::getVertices() 
{
	return m_Vertices;
}

std::vector<unsigned int> Cube::getIndices() const
{
	return s_UnitIndices;
}