#pragma once

struct Vec4;

struct Vec2 {
	float x = 0.0f, y = 0.0f;
};

struct Vec3 {
	float x = 0.0f, y = 0.0f, z = 0.0f;

	Vec3() = default;

	// regular constructor
	Vec3(float x, float y, float z);
	
	// convert vec4 to vec3
	Vec3(const Vec4& vec4);

};

struct Vec4 {
	float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

	Vec4() = default;

	// regular constructor
	Vec4(float x, float y, float z, float w);
		
	// convert vec3 to vec4
	Vec4(const Vec3& vec3, float w);

};