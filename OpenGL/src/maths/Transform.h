#pragma once

#include "maths/Vec3.h"

struct Transform {
	Vec3 tra = {0.0f, 0.0f, 0.0f};
	Vec3 rot = {0.0f, 0.0f, 0.0f};
	Vec3 sca = {1.0f, 1.0f, 1.0f};

	void operator+=(const Transform& other);
	Transform operator+(const Transform& other) const;
};

std::ostream& operator<<(std::ostream& os, const Transform& transform);