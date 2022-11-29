#include "Mesh.h"

#include <algorithm>

#include "maths/Maths.h"
#include "maths/Matrix.h"
#include "maths/Vec4.h"
#include "maths/Vec3.h"
#include "maths/Transform.h"

Mesh::Mesh(const std::vector<Vec3>& positions, const std::vector<Vec2>& texCoords, const std::vector<Vec3>& normals, const std::vector<GLuint>& indices)
	: positions(positions), textureCoordinates(texCoords), normals(normals), indices(indices)
{
}

std::vector<Vec3> Mesh::recalculatePositions(const Mat4x4& transformMatrix) const
{
	std::vector<Vec3> newPositions(positions.size());

	for (size_t i = 0; i < positions.size(); ++i)
	{
		newPositions[i] = Vec3(Vec4(positions[i], 1.0f) * transformMatrix);
	}

	return newPositions;
}

std::vector<Vec3> Mesh::recalculateNormals(const Transform& transform) const
{
	std::vector<Vec3> newNormals(normals.size());

	Mat4x4 transformMatrix = Maths::rotate(Mat4x4::identity(), transform.rot);
	transformMatrix = Maths::scale(transformMatrix, transform.sca);

	// TODO fix normal deformation on scale operations

	for (size_t i = 0; i < normals.size(); ++i)
	{
		newNormals[i] = Vec3(Vec4(normals[i], 1.0f) * transformMatrix.adjugateInverse().transpose()).normalise();
	}

	return newNormals;
}

GLuint Mesh::getMaxInt()
{
	if (m_MaxInt.has_value()) return m_MaxInt.value();

	return m_MaxInt.emplace(*std::ranges::max_element(indices));
}