#include "Object.h"

#include <numeric>
#include <optional>

#include "maths/Matrix.h"
#include "Cube.h"
#include "engine/Util.h"
#include "engine/viewport/Camera.h"

Object::Object(const Material& material)
	: material(material)
{
}

Mat4x4 Object::getTransformMatrix() const
{
	//auto m_Transform = Maths::translate(Mat4x4::identity(), m_Transform.tra.x, m_Transform.tra.y, m_Transform.tra.z);
	//m_Transform = Maths::rotate(m_Transform, m_Transform.rot.x, m_Transform.rot.y, m_Transform.rot.z);
	//m_Transform = Maths::scale(m_Transform, m_Transform.sca.x, m_Transform.sca.y, m_Transform.sca.z);

	const auto& combined = getCombinedTransformations();

	return
		Maths::scale(
			Maths::rotate(
				Maths::translate(Mat4x4::identity(), 
					combined.tra), 
				combined.rot),
			combined.sca);
}

Cube Object::getAABB() const
{
	float minX = 0, minY = 0, minZ = 0, maxX = 0, maxY = 0, maxZ = 0;
	Vec3 avg;

	for (const auto& pos : positions)
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

	const auto numPositions = static_cast<float>(positions.size());

	avg.x /= numPositions;
	avg.y /= numPositions;
	avg.z /= numPositions;

	Cube AABB(avg, 1.0f, material);
	AABB.setScale(maxX - minX, maxY - minY, maxZ - minZ);

	AABB.positions = AABB.getMesh().recalculatePositions(AABB.getTransformMatrix());

	return AABB;
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
std::optional<Vec3> Object::isRayIntersecting(const Ray& ray) const
{
	std::optional<Vec3> pointOfIntersection;
	
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

		const auto planeNormalDotRayDirection = planeNormal.dot(ray.direction);

		if (planeNormalDotRayDirection == 0) continue; // ray and triangle are parallel

		const auto d = -planeNormal.dot(v1); // distance from world origin to plane
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

void Object::setScale(float xScale, float yScale, float zScale)
{
	m_Transform.sca = { xScale, yScale, zScale };
	moved = true;
}

void Object::setScale(const Vec3& scale)
{
	setScale(scale.x, scale.y, scale.z);
}

void Object::setRotation(float xRotation, float yRotation, float zRotation)
{
	m_Transform.rot = { xRotation, yRotation, zRotation };
	moved = true;
}

void Object::setRotation(const Vec3& rotation)
{
	setRotation(rotation.x, rotation.y, rotation.z);
}

void Object::setTranslation(float xTranslate, float yTranslate, float zTranslate)
{
	m_Transform.tra = { xTranslate, yTranslate, zTranslate };
	moved = true;
}

void Object::setTranslation(const Vec3& translation)
{
	setTranslation(translation.x, translation.y, translation.z);
}

void Object::addScale(float x, float y, float z)
{
	m_Transform.sca += {x, y, z};
	moved = true;
}

void Object::addScale(const Vec3& scale)
{
	addScale(scale.x, scale.y, scale.z);
}

void Object::addRotation(float x, float y, float z)
{
	m_Transform.rot += {x, y, z};
	moved = true;
}

void Object::addRotation(const Vec3& rotation)
{
	addRotation(rotation.x, rotation.y, rotation.z);
}

void Object::addTranslation(float x, float y, float z)
{
	m_Transform.tra += {x, y, z};
	moved = true;
}

void Object::addTranslation(const Vec3& translation)
{
	addTranslation(translation.x, translation.y, translation.z);
}

void Object::offsetScale(float x, float y, float z)
{
	m_TempTransform.sca = Vec3(x, y, z);
	moved = true;
}

void Object::offsetScale(const Vec3& scale)
{
	offsetScale(scale.x, scale.y, scale.z);
}

void Object::offsetRotation(float x, float y, float z)
{
	m_TempTransform.rot = Vec3(x, y, z);
	moved = true;
}

void Object::offsetRotation(const Vec3& rotation)
{
	offsetRotation(rotation.x, rotation.y, rotation.z);
}

void Object::offsetTranslation(float x, float y, float z)
{
	m_TempTransform.tra = Vec3(x, y, z);
	moved = true;
}

void Object::offsetTranslation(const Vec3& translation)
{
	offsetTranslation(translation.x, translation.y, translation.z);
}

void Object::applyOffset()
{
	m_Transform += m_TempTransform;
	resetOffset();
}

void Object::resetOffset()
{
	m_TempTransform = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
}

const Transform& Object::getTransform() const
{
	return m_Transform;
}

const Transform& Object::getTempTransform() const
{
	return m_TempTransform;
}

Transform Object::getCombinedTransformations() const
{
	return m_Transform + m_TempTransform;
}

Vec3 Object::getAvgPosition()
{
	if (m_AvgPos.has_value() && !moved) return m_AvgPos.value();
	
	const auto& AABBPositions = getAABB().positions;

	return m_AvgPos.emplace
	(
		std::accumulate(AABBPositions.begin(), AABBPositions.end(), Vec3(0, 0, 0)) / static_cast<float>(AABBPositions.size())
	);
}