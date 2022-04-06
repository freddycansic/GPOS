#pragma once

#include <GL/glew.h>
#include "maths/Vectors.h"

struct Vertex {
	Vec3<GLfloat> position;
	//Vec4<GLfloat> colour;
	Vec2<GLfloat> textureCoordinates;
	GLfloat texID;

};