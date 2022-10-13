#pragma once

#include <array>
#include <optional>

#include "engine/rendering/object/Object.h"

class Gizmo
{
public:
	void render(const Vec3& pos) const;
	[[nodiscard]] virtual std::optional<Vec3> getIntersectingHandleAxis(const Ray& ray) const;
	[[nodiscard]] virtual std::function<void(const Object&)> getTransformation(const Vec3& axis, float magnitude) const = 0;

	Gizmo() = default;
	virtual ~Gizmo() = default;


protected:
	[[nodiscard]] virtual const std::array<Object, 3>& getHandles() const = 0;

	const static std::array<Object, 3> s_Lines;
};

class ScaleGizmo : public Gizmo
{
public:
	[[nodiscard]] std::function<void(const Object&)> getTransformation(const Vec3& axis, float magnitude) const override;

protected:
	[[nodiscard]] const std::array<Object, 3>& getHandles() const override;

private:
	const static std::array<Object, 3> s_Handles;
};

class TranslateGizmo : public Gizmo
{
public:
	[[nodiscard]] std::function<void(const Object&)> getTransformation(const Vec3& axis, float magnitude) const override;

protected:
	[[nodiscard]] const std::array<Object, 3>& getHandles() const override;

private:
	const static std::array<Object, 3> s_Handles;
};

class RotateGizmo : public Gizmo
{
public:
	[[nodiscard]] std::function<void(const Object&)> getTransformation(const Vec3& axis, float magnitude) const override;


protected:
	[[nodiscard]] const std::array<Object, 3>& getHandles() const override;

private:
	const static std::array<Object, 3> s_Handles;
};