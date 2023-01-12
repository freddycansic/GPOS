#include "Material.h"

Material::Material(const char* path, const Vec4& colour)
	: colour(colour), texturePath(path)
{
}

Material::Material(const char* path)
	: texturePath(path)
{
}

Material::Material(const Vec4& colour)
	: colour(colour)
{
}

Material::Material(const Vec3& colour)
	: colour(colour, 1.0f)
{
}