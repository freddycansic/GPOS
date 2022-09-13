#pragma once

#include <vector>

struct Vec3;
struct Vec2;
struct Transform;
class Mat4;

struct Mesh
{
	const std::vector<Vec3> positions;
	const std::vector<Vec2> textureCoordinates;
	const std::vector<unsigned int> indices;

	Mesh(const std::vector<Vec3>& positions, const std::vector<Vec2>& texCoords, const std::vector<unsigned int>& indices);

	[[nodiscard]] unsigned int getMaxInt();
	[[nodiscard]] std::vector<Vec3> recalculatePositions(const Mat4& transformMatrix) const;
	[[nodiscard]] std::vector<Vec3> recalculatePositions(const Transform& transform) const;

private:
	unsigned int m_MaxInt = std::numeric_limits<unsigned int>::max();
};