#pragma once

#include <vector>

#include "maths/Maths.h"

struct Vec3;
struct Vec2;
struct Transform;

struct Mesh
{
	const std::vector<Vec3> positions;
	const std::vector<Vec2> textureCoordinates;
	const std::vector<unsigned int> indices;

	Mesh(const std::vector<Vec3>& positions, const std::vector<Vec2>& texCoords, const std::vector<unsigned int>& indices);

	[[nodiscard]] unsigned int getMaxInt();
	[[nodiscard]] std::vector<Vec3> recalculatePositions(const Mat4x4& transformMatrix) const;
	//[[nodiscard]] std::vector<Vec3> recalculatePositions(const Transform& transform) const;

private:
	unsigned int m_MaxInt = std::numeric_limits<unsigned int>::max();
};