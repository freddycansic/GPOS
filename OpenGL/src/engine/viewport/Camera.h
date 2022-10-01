#pragma once
#include "maths/Maths.h"

struct Vec3;
struct Vec2;

//class Mat4x4;

namespace Camera
{
	enum class CameraMode
	{
		FPS_FLY,
		ORBIT
	};

	Vec3 perspRayFromCameraScreenPos(const Vec2& screenCoord);

	void update();
	void setMode(CameraMode mode);

	Vec3 getPos();
	Mat4x4 getViewMatrix();
}