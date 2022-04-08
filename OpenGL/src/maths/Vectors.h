#pragma once

struct Vec4;

struct Vec2 {
	float x, y;
};

struct Vec3 {
	float x, y, z;

	// regular constructor
	Vec3(float x, float y, float z);
	
	// convert vec4 to vec3
	Vec3(const Vec4& vec4);

};

struct Vec4 {
	float x, y, z, w;

	// regular constructor
	Vec4(float x, float y, float z, float w);
		
	// convert vec3 to vec4
	Vec4(const Vec3& vec3, float w);

};

//template<typename T>
//concept Numeric = std::floating_point<T> || std::integral<T>;
//
//template<typename T> requires Numeric<T>
//struct Vec2 {
//	T x, y;
//};
//
//template<typename T> requires Numeric<T>
//struct Vec3 {
//	T x, y, z;
//
//	// regular constructor
//	Vec3(T x, T y, T z) :
//		x(x), y(y), z(z)
//	{
//	}
//	
//	// convert vec4 to vec3
//	Vec3(const Vec4<T>& vec4) :
//		x(vec4.x), y(vec4.y), z(vec4.z)
//	{
//	}
//
//};
//
//template<typename T> requires Numeric<T>
//struct Vec4 { 
//	T x, y, z, w;
//
//	// regular constructor
//	Vec4(T x, T y, T z, T w) :
//		x(x), y(y), z(z), w(w)
//	{
//	}
//
//	// convert vec3 to vec4
//	Vec4(const Vec3<T>& vec3, T w) :
//		x(vec3.x), y(vec3.y), z(vec3.z), w(w)
//	{
//	}
//	
//	// convert array of 4 elems to vec4
//	Vec4(T values[4]) :
//		x(values[0]), y(values[1]), z(values[2]), w(values[3])
//	{
//	}
//
//};