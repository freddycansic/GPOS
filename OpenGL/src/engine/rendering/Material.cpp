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