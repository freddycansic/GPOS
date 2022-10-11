#pragma once

#include "maths/Maths.h"
#include "maths/Vectors.h"

struct Ray
{
	Vec3 origin;
	Vec3 direction;
};

namespace Camera
{
	enum class CameraMode
	{
		FPS_FLY,
		ORBIT
	};

	Ray perspRayFromCameraScreenPos(const Vec2& screenCoord);

	void update();
	void setMode(CameraMode mode);

	Vec3 getPos();
	Mat4x4 getViewMatrix();
}