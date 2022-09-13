#include "Material.h"

Material::Material(Texture&& texture, const Vec4& colour)
	: m_Texture(std::move(texture)), m_Colour(colour)
{
}

void Material::setColour(const Vec4& colour)
{
    m_Colour = colour;
}

// takes ownership of the texture
void Material::setTexture(Texture&& texture)
{
    m_Texture = std::move(texture);
}

Vec4 Material::getColour() const
{
    return m_Colour;
}

const Texture& Material::getTexture() const
{
    return m_Texture;
}
