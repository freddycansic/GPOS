#pragma once

#include "opengl/Texture.h"
#include "maths/Vectors.h"

struct Material
{
	Material(const Texture& texture, const Vec4& colour);
	Material(const Texture& texture);
	Material(const Vec4& colour);

	Vec4 colour = {1.0f, 1.0f, 1.0f, 1.0f};
	size_t texHandle = 0;
};
