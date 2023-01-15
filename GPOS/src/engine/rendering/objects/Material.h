#pragma once

#include <string>

#include "engine/Colours.h"
#include "maths/Vec4.h"

struct Vec3;

struct Material
{
	Material(const std::string& path, const Vec4& colour);
	Material(const std::string& path);
	Material(const Vec4& colour);
	Material(const Vec3& colour);
	Material() = default;

	Vec4 colour = Vec4(Colours::DEFAULT, 1.0f);
	std::string texturePath;
};