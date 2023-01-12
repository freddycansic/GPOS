#pragma once

#include "engine/rendering/opengl/Texture.h"
#include "engine/Colours.h"
#include "maths/Vec4.h"

struct Vec3;

struct Material
{
	Material(const char* path, const Vec4& colour);
	Material(const char* path);
	Material(const Vec4& colour);
	Material(const Vec3& colour);
	Material() = default;

	Vec4 colour = Vec4(Colours::DEFAULT, 1.0f);
	const char* texturePath = nullptr;
};