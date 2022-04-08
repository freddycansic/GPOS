#pragma once

#include <iostream>

#include "maths/Vectors.h"

struct Vertex {
	Vec3 position = {0, 0, 0};
	//Vec4<GLfloat> colour = {0, 0, 0, 0};
	Vec2 textureCoordinates = {0, 0};
	float texID = 0;

	Vertex(Vec3 position, Vec2 textureCoordinates, float texID);

};

std::ostream& operator<<(std::ostream& os, const Vertex& v);