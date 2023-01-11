#pragma once

#include <vector>
#include <optional>

#include <GL/glew.h>

#include "maths/Maths.h"
#include "maths/Matrix.h"
#include "Material.h"

#include "maths/Vec3.h"
#include "maths/Vec2.h"

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