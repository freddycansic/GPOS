#pragma once

#include "engine/rendering/object/shapes/Object.h"
#include "maths/Maths.h"

#include "maths/Vec3.h"

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

	Ray perspRayFromCameraScreenPos(Vec2 screenCoord);

	Vec2 NDCToScreenBounds(Vec2 NDCCoord);
	Vec2 screenToNDCBounds(Vec2 screenCoord);

	void update();
	void zoom(float direction);
	void setMode(CameraMode mode);

	void setOrbitTarget(const Vec3& target);

	Vec3 getPos();
	Mat4x4 getViewMatrix();
}