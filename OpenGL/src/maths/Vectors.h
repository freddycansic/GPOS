#pragma once

#include <concepts>

template<typename T>
concept Numeric = std::floating_point<T> || std::integral<T>;

template<typename T> requires Numeric<T>
union Vec3 {
	struct {T x, y, z;};
	struct {T r, g, b;};
};

template<typename T> requires Numeric<T>
struct Vec2 {
	T x, y;
};

