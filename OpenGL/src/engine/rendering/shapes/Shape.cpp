#include "Shape.h"

#include "maths/Matrix.h"

void Shape::setScale(float xScale, float yScale, float zScale) {
	m_Transform.sca = { xScale, yScale, zScale };
	m_Moved = true;
}

void Shape::setRotation(float xRotation, float yRotation, float zRotation) {
	m_Transform.rot = { xRotation, yRotation, zRotation };
	m_Moved = true;
}

void Shape::setTranslation(float xTranslate, float yTranslate, float zTranslate) {
	m_Transform.tra = { xTranslate, yTranslate, zTranslate };
	m_Moved = true;
}

Mat4x4 Shape::getTransformMatrix() const
{
	//auto transform = Maths::translate(Mat4x4::identity(), m_Transform.tra.x, m_Transform.tra.y, m_Transform.tra.z);
	//transform = Maths::rotate(transform, m_Transform.rot.x, m_Transform.rot.y, m_Transform.rot.z);
	//transform = Maths::scale(transform, m_Transform.sca.x, m_Transform.sca.y, m_Transform.sca.z);

	return Maths::scale(Maths::rotate(Maths::translate(Mat4x4::identity(), m_Transform.tra.x, m_Transform.tra.y, m_Transform.tra.z), m_Transform.rot.x, m_Transform.rot.y, m_Transform.rot.z), m_Transform.sca.x, m_Transform.sca.y, m_Transform.sca.z);
}

void Shape::setMoved(bool moved)
{
	m_Moved = moved;
}

bool Shape::moved() const
{
	return m_Moved;
}

void Shape::setSelected(bool selected)
{
	m_Selected = selected;
}

bool Shape::selected() const
{
	return m_Selected;
}

std::vector<Vec3> Shape::getPositions() const
{
	return m_Positions;
}

void Shape::setPositions(const std::vector<Vec3>& positions)
{
	m_Positions = positions;
}

