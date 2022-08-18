#include "Shape.h"

#include "maths/Mat4.h"

void Shape::recalculatePositions() {
	if (m_Positions.empty()) {
		m_Positions = getUnitPositions();
	}

	if (!m_Moved) return;

	const Mat4 transformMatrix = Mat4::identity.translate(m_Transform.tra.x, m_Transform.tra.y, m_Transform.tra.z).rotate(m_Transform.rot.x, m_Transform.rot.y, m_Transform.rot.z).scale(m_Transform.sca.x, m_Transform.sca.y, m_Transform.sca.z);

	const auto& unitVertices = getUnitPositions();

	for (unsigned int i = 0; i < m_Positions.size(); i++) {
		const auto& unitPos = unitVertices.at(i); // unit pos
		auto& resultPos = m_Positions.at(i); // result vertex
		
		// apply transformation to unit vertex and store in result vertex
		resultPos = Vec3(Vec4(unitPos, 1.0f) * transformMatrix);
	}

	m_Moved = false;
}

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

std::vector<Vec3> Shape::getPositions() const
{
	return m_Positions;
}