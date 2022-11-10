#include "Line.h"

#include "engine/viewport/Camera.h"
#include "maths/Maths.h"

constexpr auto PI_OVER_3 = Maths::PI<float> / 3.0f;

Mesh Line::s_Mesh =
{
	{
		{0, sinf(0 * PI_OVER_3), cosf(0 * PI_OVER_3)},
		{0, sinf(2 * PI_OVER_3), cosf(2 * PI_OVER_3)},
		{0, sinf(4 * PI_OVER_3), cosf(4 * PI_OVER_3)},
		{1, sinf(0 * PI_OVER_3), cosf(0 * PI_OVER_3)},
		{1, sinf(2 * PI_OVER_3), cosf(2 * PI_OVER_3)},
		{1, sinf(4 * PI_OVER_3), cosf(4 * PI_OVER_3)},
	},
	{
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 0.0f},
	},
	{
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	},
	{
		0, 1, 2,
		0, 3, 1,
		3, 1, 4,
		1, 4, 2,
		4, 2, 5,
		2, 5, 3,
		5, 3, 0,
		3, 4, 5,
	}
};

Line::Line(float x1, float y1, float z1, float x2, float y2, float z2, float width, const Material& material)
	: Object(material), p1(x1, y1, z1), p2(x2, y2, z2)
{
	m_Selectable = false;

	m_Transform.sca = { 1, width, width };
}

Line::Line(const Vec3& p1, const Vec3& p2, float width, const Material& material) : Line(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, width, material)
{
}

Line::Line(const Ray& ray, float length, float width, const Material& material) : Line(ray.origin, ray.origin + ray.direction * length, width, material)
{
}

const Vec3 i = { 1.0f, 0.0f, 0.0f };

Mat4x4 Line::getTransformMatrix() const
{
	const Vec3 diff = p1 - p2; // difference between the two points
	const float mag = diff.magnitude(); // mag of diff

	Vec3 axis = (i.cross(diff)).normalise();

	if (std::isnan(axis.x)) axis = { 0, 0, 0 };

	const float theta = i.angleBetween(diff);

	auto transform = Maths::translate(Mat4x4::identity(), p2 + m_Transform.tra + m_TempTransform.tra);
	transform = Maths::rotateAxis(transform, axis, theta);
	transform = Maths::scale(transform, mag, m_Transform.sca.y, m_Transform.sca.z);

	return transform;
}

Mesh& Line::getMesh() const
{
	return s_Mesh;
}