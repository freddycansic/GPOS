#pragma once

#include <memory>

#include "Material.h"
#include "shapes/Shape.h"

struct Object
{
	Object(std::unique_ptr<Shape>&& shapePtr, const Material& material);

	std::unique_ptr<Shape> shapePtr;
	Material material;
};
