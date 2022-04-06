#pragma once

#include <concepts>
#include <iostream>

template<typename T>
concept Numeric = std::floating_point<T> || std::integral<T>;

template<typename T>
struct Vec2;

template<typename T> requires Numeric<T>
struct Vec2 {
	T x, y;
};

template<typename T> requires Numeric<T>
struct Vec3 {
	T x, y, z;

	Vec3(T x, T y, T z) :
		x(x), y(y), z(z)
	{
	}

	Vec3(Vec4<T> vec4) :
		x(vec4.x), y(vec4.y), z(vec4.z)
	{
	}
};

template<typename T> requires Numeric<T>
struct Vec4 { 
	T x, y, z, w;

	Vec4(T x, T y, T z, T w) :
		x(x), y(y), z(z), w(w)
	{
	}

	Vec4(T values[4]) :
		x(values[0]), y(values[1]), z(values[2]), w(values[3])
	{
	}

	Vec4(Vec3<T> vec3, T w) :
		x(vec3.x), y(vec3.y), z(vec3.z), w(w)
	{
	}

};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vec4<T>& vector) {
	return os << "| " << vector.x << " |\n"
			  << "| " << vector.y << " |\n"
			  << "| " << vector.z << " |\n"
			  << "| " << vector.w << " |\n";
}