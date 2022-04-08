#include "Shape.h"

void Shape::setTexID(unsigned int ID)
{
	for (auto& vertex : vertices) {
		vertex.texID = ID;
	}
}
