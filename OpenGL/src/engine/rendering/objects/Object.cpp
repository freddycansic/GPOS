#include "Object.h"

#include "maths/Matrix.h"
#include "Cube.h"
#include "engine/viewport/Camera.h"

void Object::setScale(float xScale, float yScale, float zScale) {
	m_Transform.sca = { xScale, yScale, zScale };
	m_Moved = true;
}

void Object::setRotation(float xRotation, float yRotation, float zRotation) {
	m_Transform.rot = { xRotation, yRotation, zRotation };
	m_Moved = true;
}

void Object::setTranslation(float xTranslate, float yTranslate, float zTranslate) {
	m_Transform.tra = { xTranslate, yTranslate, zTranslate };
	m_Moved = true;
}

Mat4x4 Object::getTransformMatrix() const
{
	//auto transform = Maths::translate(Mat4x4::identity(), m_Transform.tra.x, m_Transform.tra.y, m_Transform.tra.z);
	//transform = Maths::rotate(transform, m_Transform.rot.x, m_Transform.rot.y, m_Transform.rot.z);
	//transform = Maths::scale(transform, m_Transform.sca.x, m_Transform.sca.y, m_Transform.sca.z);

	return Maths::scale(Maths::rotate(Maths::translate(Mat4x4::identity(), m_Transform.tra.x, m_Transform.tra.y, m_Transform.tra.z), m_Transform.rot.x, m_Transform.rot.y, m_Transform.rot.z), m_Transform.sca.x, m_Transform.sca.y, m_Transform.sca.z);
}

Cube Object::getAABB() const
{
	float minX = 0, minY = 0, minZ = 0, maxX = 0, maxY = 0, maxZ = 0;
	Vec3 avg;

	for (const auto& pos : m_Positions)
	{
		minX = pos.x < minX ? pos.x : minX;
		minY = pos.y < minY ? pos.y : minY;
		minZ = pos.z < minZ ? pos.z : minZ;
		maxX = pos.x > maxX ? pos.x : maxX;
		maxY = pos.y > maxY ? pos.y : maxY;
		maxZ = pos.z > maxZ ? pos.z : maxZ;

		avg.x += pos.x;
		avg.y += pos.y;
		avg.z += pos.z;
	}

	const auto numPositions = static_cast<float>(m_Positions.size());

	avg.x /= numPositions;
	avg.y /= numPositions;
	avg.z /= numPositions;

	Cube AABB(avg, 1.0f);
	AABB.setScale(maxX - minX, maxY - minY, maxZ - minZ);

	return AABB;
}

bool Object::isRayIntersecting(const Ray& ray) const
{
	const auto& mesh = this->getMesh();

	for (size_t i = 0; i < mesh.indices.size(); i+=3)
	{
		const auto& i1 = mesh.indices.at(i);
		const auto& i2 = mesh.indices.at(i + 1);
		const auto& i3 = mesh.indices.at(i + 2);

		const auto& v1 = mesh.positions.at(i1);
		const auto& v2 = mesh.positions.at(i2);
		const auto& v3 = mesh.positions.at(i3);

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
			planeNormal.dot(edge3.cross(c3)) > 0) return true;
	}

	return false;
}

void Object::setMoved(bool moved)
{
	m_Moved = moved;
}

bool Object::moved() const
{
	return m_Moved;
}

void Object::setSelected(bool selected)
{
	m_Selected = selected;
}

bool Object::selected() const
{
	return m_Selected;
}

void Object::setSelectable(bool selectable)
{
	m_Selectable = selectable;
}

bool Object::selectable() const
{
	return m_Selectable;
}

std::vector<Vec3> Object::getPositions() const
{
	return m_Positions;
}

void Object::setPositions(const std::vector<Vec3>& positions)
{
	m_Positions = positions;
}

