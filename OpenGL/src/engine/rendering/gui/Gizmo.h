#pragma once

#include <array>

#include "engine/rendering/object/Object.h"

class Gizmo
{
public:
	void render() const;

protected:
	Gizmo() = default;
	virtual ~Gizmo() = default;

	[[nodiscard]] virtual const std::array<Object, 3>& getHandles() const = 0;

	const static std::array<Object, 3> s_Lines;
};

class ScaleGizmo : public Gizmo
{
public:
	explicit ScaleGizmo(const Vec3& pos);

protected:
	[[nodiscard]] const std::array<Object, 3>& getHandles() const override;

private:
	const static std::array<Object, 3> s_Handles;
};