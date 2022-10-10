#include "Mesh.h"

#include <algorithm>

#include "maths/Maths.h"
#include "maths/Matrix.h"
#include "maths/Vectors.h"
#include "maths/Transform.h"

Mesh::Mesh(const std::vector<Vec3>& positions, const std::vector<Vec2>& texCoords, const std::vector<unsigned int>& indices) :
	positions(positions), textureCoordinates(texCoords), indices(indices)
{
}

std::vector<Vec3> Mesh::recalculatePositions(const Mat4x4& transformMatrix) const
{
	std::vector<Vec3> newPositions(positions.size());

	for (unsigned int i = 0; i < positions.size(); ++i)
	{
		newPositions[i] = Vec3(Vec4(positions[i], 1.0f) * transformMatrix);
	}

	return newPositions;
}

unsigned int Mesh::getMaxInt()
{
	if (m_MaxInt != std::numeric_limits<unsigned int>::max()) return m_MaxInt;

	m_MaxInt = *std::ranges::max_element(indices);

	return m_MaxInt;
}
