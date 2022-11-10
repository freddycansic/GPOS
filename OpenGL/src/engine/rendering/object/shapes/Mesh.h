#pragma once

#include <vector>
#include <optional>

#include "maths/Maths.h"

struct Vec3;
struct Vec2;
struct Transform;

struct Mesh
{
	const std::vector<Vec3> positions;
	const std::vector<Vec2> textureCoordinates;
	const std::vector<Vec3> normals;
	const std::vector<unsigned int> indices;

	Mesh(const std::vector<Vec3>& positions, const std::vector<Vec2>& texCoords, const std::vector<Vec3>& normals, const std::vector<unsigned int>& indices);
	Mesh() = default;

	[[nodiscard]] unsigned int getMaxInt();
	[[nodiscard]] std::vector<Vec3> recalculatePositions(const Mat4x4& transformMatrix) const;
	[[nodiscard]] std::vector<Vec3> recalculateNormals(const Transform& transform) const;

private:
	std::optional<unsigned int> m_MaxInt;
};