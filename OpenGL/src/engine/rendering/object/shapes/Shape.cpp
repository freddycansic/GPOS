#include "Shape.h"

#include <optional>

#include "maths/Matrix.h"
#include "Cube.h"
#include "engine/Util.h"
#include "engine/viewport/Camera.h"

Mat4x4 Shape::getTransformMatrix() const
{
	//auto transform = Maths::translate(Mat4x4::identity(), m_Transform.tra.x, m_Transform.tra.y, m_Transform.tra.z);
	//transform = Maths::rotate(transform, m_Transform.rot.x, m_Transform.rot.y, m_Transform.rot.z);
	//transform = Maths::scale(transform, m_Transform.sca.x, m_Transform.sca.y, m_Transform.sca.z);

	return Maths::scale(Maths::rotate(Maths::translate(Mat4x4::identity(), m_Transform.tra.x, m_Transform.tra.y, m_Transform.tra.z), m_Transform.rot.x, m_Transform.rot.y, m_Transform.rot.z), m_Transform.sca.x, m_Transform.sca.y, m_Transform.sca.z);
}

Cube Shape::getAABB() const
{
	return Util::getAABBFromPoints(m_Positions);
}

std::optional<Vec3> Shape::isRayIntersecting(const Ray& ray) const
{
	std::optional<Vec3> pointOfIntersection;

	const auto& positions = this->getPositions();
	const auto& indices = this->getMesh().indices;

	for (size_t i = 0; i < indices.size(); i+=3)
	{
		const auto& i1 = indices.at(i);
		const auto& i2 = indices.at(i + 1);
		const auto& i3 = indices.at(i + 2);

		const auto& v1 = positions.at(i1);
		const auto& v2 = positions.at(i2);
		const auto& v3 = positions.at(i3);

		const auto v2MinusV1 = v2 - v1;
		const auto v3MinusV1 = v3 - v1;

		const auto planeNormal = v2MinusV1.cross(v3MinusV1);

		const auto d = -planeNormal.dot(v1); // distance from origin to plane

		const auto planeNormalDotRayDirection = planeNormal.dot(ray.direction);

		if (planeNormalDotRayDirection == 0) continue; // ray and triangle are parallel

		const auto rayOriginToIntersectionDist = -(planeNormal.dot(ray.origin) + d) / planeNormalDotRayDirection;

		if (rayOriginToIntersectionDist < 0) continue; // ray direction is going away from the triangle

		const auto intersection = ray.origin + ray.direction * rayOriginToIntersectionDist;

		const auto edge1 = v2 - v1;
		const auto edge2 = v3 - v2;
		const auto edge3 = v1 - v3;

		const auto c1 = intersection - v1;
		const auto c2 = intersection - v2;
		const auto c3 = intersection - v3;

		if (planeNormal.dot(edge1.cross(c1)) > 0 &&
			planeNormal.dot(edge2.cross(c2)) > 0 &&
			planeNormal.dot(edge3.cross(c3)) > 0) {

			pointOfIntersection.emplace(intersection);
			return pointOfIntersection;
		}
	}

	return pointOfIntersection;
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

void Shape::addScale(float x, float y, float z)
{
	m_Transform.sca += {x, y, z};
	m_Moved = true;
}

void Shape::addRotation(float x, float y, float z)
{
	m_Transform.rot += {x, y, z};
	m_Moved = true;
}

void Shape::addTranslation(float x, float y, float z)
{
	m_Transform.tra += {x, y, z};
	m_Moved = true;
}

Vec3 Shape::getAvgPosition()
{
	if (m_AvgPos.x != std::numeric_limits<float>::max()) return m_AvgPos;

	Vec3 avg;
	for (const auto& pos : m_Positions)
	{
		avg += pos;
	}

	avg /= static_cast<float>(m_Positions.size());

	m_AvgPos = avg;
	return avg;
}

void Shape::setMoved(bool moved)
{
	m_Moved = moved;
}

bool Shape::moved() const
{
	return m_Moved;
}

const std::vector<Vec3>& Shape::getPositions() const
{
	return m_Positions;
}

void Shape::setPositions(const std::vector<Vec3>& positions)
{
	m_Positions = positions;
}

