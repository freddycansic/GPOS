#include "Cube.h"

Mesh Cube::s_Mesh =
{
	{
		{-0.5f, -0.5f,  0.5f}, // 0 bottom front left
		{-0.5f, -0.5f,  0.5f}, // 0 bottom front left
		{-0.5f, -0.5f,  0.5f}, // 0 bottom front left
		{ 0.5f, -0.5f,  0.5f}, // 1 bottom front right
		{ 0.5f, -0.5f,  0.5f}, // 1 bottom front right
		{ 0.5f, -0.5f,  0.5f}, // 1 bottom front right
		{ 0.5f, -0.5f, -0.5f}, // 2 bottom back right
		{ 0.5f, -0.5f, -0.5f}, // 2 bottom back right
		{ 0.5f, -0.5f, -0.5f}, // 2 bottom back right
		{-0.5f, -0.5f, -0.5f}, // 3 bottom back left
		{-0.5f, -0.5f, -0.5f}, // 3 bottom back left
		{-0.5f, -0.5f, -0.5f}, // 3 bottom back left
		{-0.5f,  0.5f,  0.5f}, // 4 top front left
		{-0.5f,  0.5f,  0.5f}, // 4 top front left
		{-0.5f,  0.5f,  0.5f}, // 4 top front left
		{ 0.5f,  0.5f,  0.5f}, // 5 top front right
		{ 0.5f,  0.5f,  0.5f}, // 5 top front right
		{ 0.5f,  0.5f,  0.5f}, // 5 top front right
		{ 0.5f,  0.5f, -0.5f}, // 6 top back right
		{ 0.5f,  0.5f, -0.5f}, // 6 top back right
		{ 0.5f,  0.5f, -0.5f}, // 6 top back right
		{-0.5f,  0.5f, -0.5f}, // 7 top back left
		{-0.5f,  0.5f, -0.5f}, // 7 top back left
		{-0.5f,  0.5f, -0.5f}  // 7 top back left
	},
	{
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, 1.0f},
		{1.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 0.0f}
	},
	{
		1, 4, 7, // bottom front
		1, 7, 10, // bottom back
		13, 16, 19, // top front
		13, 19, 22, // top back
		0, 9, 12, // left front
		9, 12, 21, // left back
		3, 6, 15, // right front
		6, 15, 18, // right back
		11, 8, 20, // back right
		11, 20, 23, // back left
		2, 5, 17, // front right
		2, 17, 14 // front left
	},
	{
		{-1, 0, 0}, // 0 bottom front left
		{0, -1, 0}, // 1 bottom front left
		{0, 0, 1},  // 2 bottom front left
		{1, 0, 0},  // 3 bottom front right
		{0, -1, 0}, // 4 bottom front right
		{0, 0, 1},  // 5 bottom front right
		{1, 0, 0},  // 6 bottom back right
		{0, -1, 0}, // 7 bottom back right
		{0, 0, -1}, // 8 bottom back right
		{-1, 0, 0}, // 9 bottom back left
		{0, -1, 0}, // 10 bottom back left
		{0, 0, -1}, // 11 bottom back left
		{-1, 0, 0}, // 12 top front left
		{0, 1, 0},  // 13 top front left
		{0, 0, 1},  // 14 top front left
		{1, 0, 0},  // 15 top front right
		{0, 1, 0},  // 16 top front right
		{0, 0, 1},  // 17 top front right
		{1, 0, 0},  // 18 top back right
		{0, 1, 0},  // 19 top back right
		{0, 0, -1}, // 20 top back right
		{-1, 0, 0}, // 21 top back left
		{0, 1, 0},  // 22 top back left
		{0, 0, -1}  // 23 top back left
	}
};

Cube::Cube(float x, float y, float z, float size, const Material& material)
	: Object(material), m_Size(size)
{
	m_Transform.tra = { x, y, z };
	m_Transform.rot = { 0.0f, 0.0f, 0.0f };
	m_Transform.sca = { size, size, size };
}

Cube::Cube(const Vec3& pos, float size, const Material& material) : Cube(pos.x, pos.y, pos.z, size, material)
{
}

void Cube::setScale(float xScale, float yScale, float zScale) {
	m_Transform.sca = { m_Size * xScale, m_Size * yScale, m_Size * zScale };
	moved = true;
}

Mesh& Cube::getMesh() const {
	return s_Mesh;
}