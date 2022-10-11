#include "Material.h"

Material::Material(const Texture& texture, const Vec4& colour)
	: colour(colour), texHandle(texture.getHandle())
{
}

Material::Material(const Texture& texture)
	: texHandle(texture.getHandle())
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