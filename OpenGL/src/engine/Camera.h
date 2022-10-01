#pragma once

struct Vec3;
struct Vec2;

//class Mat4x4;

namespace Camera
{
	Vec3 perspRayFromScreenPos(const Vec2& screenCoord, const Vec3& rayOrigin);

	//Mat4x4 getViewMatrix();
}