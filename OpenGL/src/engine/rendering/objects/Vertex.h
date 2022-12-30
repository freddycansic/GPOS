#pragma once

#include <iostream>

#include "maths/Vec2.h"
#include "maths/Vec3.h"
#include "maths/Vec4.h"

struct Vertex {
	Vec3 position = {0, 0, 0};
	Vec4 color = {0, 0, 0, 0};
	Vec2 textureCoordinates = {0, 0};
	Vec3 normal = { 0, 0, 0 };
};

std::ostream& operator<<(std::ostream& os, const Vertex& v);