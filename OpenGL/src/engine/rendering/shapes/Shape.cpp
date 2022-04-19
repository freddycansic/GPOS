#include "Shape.h"

#include "maths/Vectors.h"
#include "maths/Mat4.h"

void Shape::setRotation(float xRotation, float yRotation, float zRotation)
{
	for (auto& vertex : vertices) {
		vertex.position = Mat4::identity.rotate(xRotation, yRotation, zRotation) * Vec4(vertex.position, 1.0f);
	}
}
