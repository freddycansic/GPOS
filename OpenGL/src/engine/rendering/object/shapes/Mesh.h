#pragma once

#include <vector>
#include <optional>

#include <GLEW/glew.h>

#include "maths/Maths.h"

struct Vec3;
struct Vec2;
struct Transform;

struct Mesh
{
	const std::vector<Vec3> positions;
	const std::vector<Vec2> textureCoordinates;
	const std::vector<Vec3> normals;
	const std::vector<GLuint> indices;

	Mesh(const std::vector<Vec3>& positions, const std::vector<Vec2>& texCoords, const std::vector<Vec3>& normals, const std::vector<GLuint>& indices);
	Mesh() = default;

	[[nodiscard]] GLuint getMaxInt();
	[[nodiscard]] std::vector<Vec3> recalculatePositions(const Mat4x4& transformMatrix) const;
	[[nodiscard]] std::vector<Vec3> recalculateNormals(const Transform& transform) const;

private:
	std::optional<GLuint> m_MaxInt;
};