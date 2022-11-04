#pragma once

#include <array>
#include <optional>

#include "engine/rendering/object/shapes/Line.h"
#include "engine/rendering/object/shapes/Object.h"
#include "maths/Vectors.h"

// TODO AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

class Gizmo
{
public:
	void render(const Vec3& pos) const;
	[[nodiscard]] virtual std::optional<Vec3> getIntersectingHandleAxis(const Ray& ray) const;
	[[nodiscard]] virtual std::function<void(Object&)> getSetTransformation(const Vec3& axis, float magnitude) const = 0;
	[[nodiscard]] virtual std::function<void(Object&)> getOffsetTransformation(const Vec3& axis, float magnitude) const = 0;

	Gizmo() = default;
	virtual ~Gizmo() = default;


protected:
	[[nodiscard]] virtual const std::array<std::unique_ptr<Object>, 3>& getHandles() const = 0;

	static std::array<Line, 3> s_Lines;
};

class ScaleGizmo : public Gizmo
{
public:
	[[nodiscard]] std::function<void(Object&)> getSetTransformation(const Vec3& axis, float magnitude) const override;
	[[nodiscard]] std::function<void(Object&)> getOffsetTransformation(const Vec3& axis, float magnitude) const override;

protected:
	[[nodiscard]] const std::array<std::unique_ptr<Object>, 3>& getHandles() const override;

private:
	static std::array<std::unique_ptr<Object>, 3> s_Handles;
};

class TranslateGizmo : public Gizmo
{
public:
	[[nodiscard]] std::function<void(Object&)> getSetTransformation(const Vec3& axis, float magnitude) const override;
	[[nodiscard]] std::function<void(Object&)> getOffsetTransformation(const Vec3& axis, float magnitude) const override;

protected:
	[[nodiscard]] const std::array<std::unique_ptr<Object>, 3>& getHandles() const override;

private:
	static std::array<std::unique_ptr<Object>, 3> s_Handles;
};

class RotateGizmo : public Gizmo
{
public:
	[[nodiscard]] std::function<void(Object&)> getSetTransformation(const Vec3& axis, float magnitude) const override;
	[[nodiscard]] std::function<void(Object&)> getOffsetTransformation(const Vec3& axis, float magnitude) const override;


protected:
	[[nodiscard]] const std::array<std::unique_ptr<Object>, 3>& getHandles() const override;

private:
	static std::array<std::unique_ptr<Object>, 3> s_Handles;
};