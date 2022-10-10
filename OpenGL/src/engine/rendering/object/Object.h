#pragma once

#include <memory>

#include "Material.h"
#include "shapes/Shape.h"

struct Object
{
	std::unique_ptr<Shape> shape = nullptr;
	Material material;
};
