#pragma once

#include "maths/Vectors.h"
#include "opengl/Texture.h"

class Material
{
public:
	Material() = default;
	Material(Texture&& texture, const Vec4& colour);

	void setColour(const Vec4& colour);
	void setTexture(Texture&& texture);

	[[nodiscard]] Vec4 getColour() const;
	[[nodiscard]] const Texture& getTexture() const;

private:
	Texture m_Texture;
	Vec4 m_Colour;
};

