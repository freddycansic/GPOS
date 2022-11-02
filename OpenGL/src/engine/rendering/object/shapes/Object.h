#pragma once

#include <optional>

#include "Mesh.h"
#include "engine/rendering/object/Material.h"
#include "maths/Maths.h"
#include "maths/Transform.h"

struct Cube;
struct Vec3;
struct Vec2;
struct Ray;

struct Object
{
public:
	std::vector<Vec3> positions;
	Material material;
	bool selected = false;
	bool moved = true;

protected:
	Transform m_Transform;
	std::optional<Vec3> m_AvgPos;
	bool m_Selectable = true;

public:
	Object() = default;
	explicit Object(const Material& material);
	virtual ~Object() = default;

	virtual void setScale(float xScale, float yScale, float zScale);
	virtual void setRotation(float xRotation, float yRotation, float zRotation);
	virtual void setTranslation(float xTranslate, float yTranslate, float zTranslate);

	virtual void addScale(float x, float y, float z);
	virtual void addRotation(float x, float y, float z);
	virtual void addTranslation(float x, float y, float z);

	virtual void offsetScale(float x, float y, float z);
	virtual void offsetRotation(float x, float y, float z);
	virtual void offsetTranslation(float x, float y, float z);

	[[nodiscard]] Cube getAABB() const;
	[[nodiscard]] Vec3 getAvgPosition(); // centre of mass

	[[nodiscard]] std::optional<Vec3> isRayIntersecting(const Ray& ray) const;
	
	[[nodiscard]] virtual Mesh& getMesh() const = 0; // should to point to static Mesh in child class
	[[nodiscard]] virtual Mat4x4 getTransformMatrix() const;
};