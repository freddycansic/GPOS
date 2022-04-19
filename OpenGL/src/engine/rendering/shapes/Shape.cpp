#include "Shape.h"

#include "maths/Mat4.h"

void Shape::recalculateVertices()
{
	// goes through each vertex in shape and applies scale rotation and translation to it
	for (auto& vertex : m_Vertices) {
		std::cout << vertex << std::endl;
		
		vertex.position = Vec3(Mat4::identity.scale(m_Scale.x, m_Scale.y, m_Scale.z).rotate(m_Rotation.x, m_Rotation.y, m_Rotation.z).translate(m_Translation.x, m_Translation.y, m_Translation.z) * Vec4(vertex.position, 1.0f));

		std::cout << vertex << "\n" << std::endl;

	}
}

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