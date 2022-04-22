#include "Shape.h"

#include "maths/Mat4.h"

void Shape::setRotation(float xRotation, float yRotation, float zRotation) {
	m_Rotation = { xRotation, yRotation, zRotation };
	recalculateVertices();
}

void Shape::setScale(float xScale, float yScale, float zScale) {
	m_Scale = { xScale, yScale, zScale };
	recalculateVertices();
}

void Shape::setTranslate(float xTranslate, float yTranslate, float zTranslate) {
	m_Translation = { xTranslate, yTranslate, zTranslate };
	recalculateVertices();
}