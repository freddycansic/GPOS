#pragma once

#include <optional>
#include <memory>
#include <string>

#include "Mesh.h"
#include "Material.h"
#include "maths/Maths.h"
#include "maths/Transform.h"
#include "maths/Vec3.h"

struct Cube;
struct Vec2;
struct Ray;

struct Object
{
public:
	std::vector<Vec3> positions;
	std::vector<Vec3> normals;

	Material material;
	bool selected = false;
	bool moved = true;

protected:
	Transform m_Transform;
	Transform m_TempTransform =
	{
		{0, 0, 0},
		{0, 0, 0},
		{0, 0, 0}
	};

	std::optional<Vec3> m_AvgPos;
	bool m_Selectable = true;

public:
	Object() = default;
	explicit Object(const Material& material);
	virtual ~Object() = default;

	[[nodiscard]] virtual std::unique_ptr<Object> clone() const = 0;
	[[nodiscard]] virtual std::string stringName() const = 0;

	virtual void setScale(float xScale, float yScale, float zScale);
	virtual void setScale(const Vec3& scale);
	virtual void setRotation(float xRotation, float yRotation, float zRotation);
	virtual void setRotation(const Vec3& rotation);
	virtual void setTranslation(float xTranslate, float yTranslate, float zTranslate);
	virtual void setTranslation(const Vec3& translation);

	virtual void addScale(float x, float y, float z);
	virtual void addScale(const Vec3& scale);
	virtual void addRotation(float x, float y, float z);
	virtual void addRotation(const Vec3& rotation);
	virtual void addTranslation(float x, float y, float z);
	virtual void addTranslation(const Vec3& translation);

	virtual void offsetScale(float x, float y, float z);
	virtual void offsetScale(const Vec3& scale);
	virtual void offsetRotation(float x, float y, float z);
	virtual void offsetRotation(const Vec3& rotation);
	virtual void offsetTranslation(float x, float y, float z);
	virtual void offsetTranslation(const Vec3& translation);

	void applyOffset();
	void resetOffset();

	[[nodiscard]] const Transform& getTransform() const;
	[[nodiscard]] const Transform& getTempTransform() const;
	[[nodiscard]] Transform getCombinedTransformations() const;

	[[nodiscard]] Cube getAABB() const;
	[[nodiscard]] Vec3 getAvgPosition(); // centre of mass

	[[nodiscard]] std::optional<Vec3> isRayIntersecting(const Ray& ray) const;
	
	[[nodiscard]] virtual Mesh& getMesh() const = 0; // should to point to static Mesh in child class
	[[nodiscard]] virtual Mat4x4 getTransformMatrix() const;
};