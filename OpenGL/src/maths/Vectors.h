#pragma once

template<typename T>
union Vec3 {
	struct {T x, y, z;};
	struct {T r, g, b;};
};

template<typename T>
struct Vec2 {
	T x, y;
};

