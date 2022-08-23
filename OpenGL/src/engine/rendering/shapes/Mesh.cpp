#include "Mesh.h"

#include "maths/Mat4.h"
#include "maths/Vectors.h"
#include "maths/Transform.h"

Mesh::Mesh(const std::vector<Vec3>& positions, const std::vector<Vec2>& texCoords, const std::vector<unsigned int>& indices) :
	positions(positions), textureCoordinates(texCoords), indices(indices)
{
}

std::vector<Vec3> Mesh::recalculatePositions(const Mat4& transformMatrix) const {

	std::vector<Vec3> newPositions(positions.size());

	for (unsigned int i = 0; i < positions.size(); ++i)
	{
		newPositions[i] = Vec3(Vec4(positions[i], 1.0f) * transformMatrix);
	}

	return newPositions;
}

std::vector<Vec3> Mesh::recalculatePositions(const Transform& transform) const
{
	return recalculatePositions(Mat4::identity.translate(transform.tra.x, transform.tra.y, transform.tra.z).rotate(transform.rot.x, transform.rot.y, transform.rot.z).scale(transform.sca.x, transform.sca.y, transform.sca.z));
}

