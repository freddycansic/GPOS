#include "Cube.h"

const std::vector<Vec3> Cube::s_UnitPositions =
{
	{ -0.5f, -0.5f,  0.5f }, // 0 bottom front left
	{  0.5f, -0.5f,  0.5f }, // 1 bottom front right
	{  0.5f, -0.5f, -0.5f }, // 2 bottom back right
	{ -0.5f, -0.5f, -0.5f }, // 3 bottom back left
	{ -0.5f,  0.5f,  0.5f }, // 4 top front left
	{  0.5f,  0.5f,  0.5f }, // 5 top front right
	{  0.5f,  0.5f, -0.5f }, // 6 top back right
	{ -0.5f,  0.5f, -0.5f } // 7 top back left
};

const std::vector<Vec2> Cube::s_TextureCoordinates =
{
	{0.0f, 0.0f},
	{0.0f, 1.0f},
	{1.0f, 1.0f},
	{1.0f, 0.0f},
	{0.0f, 0.0f},
	{0.0f, 1.0f},
	{1.0f, 1.0f},
	{1.0f, 0.0f} 
};

const std::vector<unsigned int> Cube::s_Indices =
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

	recalculatePositions();
}

Cube::Cube(const Vec3& pos, float size) : Cube(pos.x, pos.y, pos.z, size)
{
}

void Cube::setScale(float xScale, float yScale, float zScale) {
	m_Transform.sca = { m_Size * xScale, m_Size * yScale, m_Size * zScale };
	m_Moved = true;
}

const std::vector<Vec3>& Cube::getUnitPositions() const {
	return s_UnitPositions;
}

const std::vector<unsigned int>& Cube::getIndices() const
{
	return s_Indices;
}

const std::vector<Vec2>& Cube::getTextureCoordinates() const
{
	return s_TextureCoordinates;
}
