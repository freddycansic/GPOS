#include "Shape.h"

void Shape::setScale(float xScale, float yScale, float zScale) {
	m_Transform.sca = { xScale, yScale, zScale };
}

void Shape::setRotation(float xRotation, float yRotation, float zRotation) {
	m_Transform.rot = { xRotation, yRotation, zRotation };
	m_Moved = true;
}

void Shape::setTranslation(float xTranslate, float yTranslate, float zTranslate) {
	m_Transform.tra = { xTranslate, yTranslate, zTranslate };
}

// FIX ** TEMPORARY ** TODO PLZ
float* Shape::scalePtr()
{
	return &m_Transform.sca.x;
}

float* Shape::rotationPtr()
{
	return &m_Transform.rot.x;
}

float* Shape::translationPtr()
{
	return &m_Transform.tra.x;
}
// PLZ

Mat4 Shape::getTransformMatrix() const
{
	// WRONG ORDER DONT USE
	//const auto& translated = Mat4::identity.translate(m_Transform.tra.x, m_Transform.tra.y, m_Transform.tra.z);
	//const auto& rotated = translated.rotate(m_Transform.rot.x, m_Transform.rot.y, m_Transform.rot.z);
	//const auto& scaled = rotated.scale(m_Transform.sca.x, m_Transform.sca.y, m_Transform.sca.z).scale(m_Transform.sca.x, m_Transform.sca.y, m_Transform.sca.z);

	//return scaled;

	return Mat4::identity.translate(m_Transform.tra.x, m_Transform.tra.y, m_Transform.tra.z).rotate(m_Transform.rot.x, m_Transform.rot.y, m_Transform.rot.z).scale(m_Transform.sca.x, m_Transform.sca.y, m_Transform.sca.z);
}

void Shape::setMoved(bool moved)
{
	m_Moved = moved;
}

bool Shape::moved() const
{
	return m_Moved;
}

std::vector<Vec3> Shape::getPositions() const
{
	return m_Positions;
}

void Shape::setPositions(const std::vector<Vec3>& positions)
{
	m_Positions = positions;
}

