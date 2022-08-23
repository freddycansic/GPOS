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

	// TODO add some sort of caching for maxIndex of indices to reduce the searching in ShapeRenderer
	//unsigned int maxIndex = -1;

	Mesh(const std::vector<Vec3>& positions, const std::vector<Vec2>& texCoords, const std::vector<unsigned int>& indices);

	[[nodiscard]] std::vector<Vec3> recalculatePositions(const Mat4& transformMatrix) const;
	[[nodiscard]] std::vector<Vec3> recalculatePositions(const Transform& transform) const;
};