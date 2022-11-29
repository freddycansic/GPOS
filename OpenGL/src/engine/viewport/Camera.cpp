#include "Camera.h"

#include "engine/Window.h"
#include "engine/input/Input.h"
#include "engine/input/Keys.h"
#include "maths/Vec3.h"
#include "engine/rendering/Renderer.h"

Camera::CameraMode s_Mode = Camera::CameraMode::ORBIT;
Mat4x4 s_View = Mat4x4::identity();

Vec3 s_CameraPos = { 0.0f, 0.0f, 15.0f };

Mat4x4 fpsFlyUpdate();
Vec3 s_CameraFront = { 0.0f, 0.0f, 1.0f }; // point forward

Mat4x4 orbitUpdate();
float s_OrbitRadius = 10.0f;
Vec3 s_OrbitTarget;

namespace Camera
{
	Vec2 NDCToScreenBounds(Vec2 NDCCoord)
	{
		const auto w = static_cast<float>(Window::width());
		const auto h = static_cast<float>(Window::height());

		return
		{
			(NDCCoord.x + 1.0f) * w / 2.0f,
			(NDCCoord.y - 1.0f) * h / -2.0f
		};
	}

	Vec2 screenToNDCBounds(Vec2 screenCoord)
	{
		const auto w = static_cast<float>(Window::width());
		const auto h = static_cast<float>(Window::height());

		return
		{
			(2.0f * screenCoord.x / w - 1.0f),
			1.0f - 2.0f * screenCoord.y / h
		};
	}

	// https://stackoverflow.com/questions/45893277/is-it-possible-get-which-surface-of-cube-will-be-click-in-opengl/45946943#45946943
	Ray perspRayFromCameraScreenPos(Vec2 screenCoord)
	{
		const auto NDCPos = screenToNDCBounds(screenCoord);

		const Vec3 directionView = Vec3(Vec4(NDCPos.x, NDCPos.y, 0, 1) * Renderer::getProjectionMatrix().adjugateInverse()).normalise();

		const auto invView = s_View.adjugateInverse();

		const Vec3 rayWorldStart = Vec3(Vec4(s_CameraPos) * invView);
		const Vec3 rayWorldEnd = Vec3(Vec4(s_CameraPos + directionView) * invView);

		return {getPos(), (rayWorldEnd - rayWorldStart).normalise()};
	}

	void update()
	{
		switch (s_Mode)
		{
		case CameraMode::FPS_FLY: {
			s_View = fpsFlyUpdate();
			break;
		}

		case CameraMode::ORBIT:
			s_View = orbitUpdate();
			break;
		}
	}

	void zoom(float direction)
	{
		switch (s_Mode)
		{
		case CameraMode::FPS_FLY:
		{
			s_CameraPos += s_CameraFront * -direction;
			break;
		}
		case CameraMode::ORBIT:
		{
			static constexpr float ORBIT_ZOOM_STEP = 0.5f;
			s_OrbitRadius += ORBIT_ZOOM_STEP * -direction;
			Camera::update();
			break;
		}
		}
	}

	void setMode(CameraMode mode)
	{
		s_Mode = mode;
	}

	void setOrbitTarget(const Vec3& target)
	{
		s_OrbitTarget = target;
	}

	Vec3 getPos()
	{
		return s_CameraPos;
	}

	Mat4x4 getViewMatrix()
	{
		return s_View;
	}
}

Mat4x4 fpsFlyUpdate()
{
	static Vec3 ls_CameraUp = { 0.0f, 1.0f, 0.0f };

	constexpr static float ls_SprintSpeed = 2.0f;
	const float moveSpeed = 10.0f * (Input::isKeyDown(Keys::LEFT_SHIFT) ? ls_SprintSpeed : 1.0f);

	s_CameraFront = Input::getCameraDirection();

	if (Input::isKeyDown(Keys::W)) {
		s_CameraPos += s_CameraFront * moveSpeed * Window::deltatime();
	}
	if (Input::isKeyDown(Keys::S)) {
		s_CameraPos -= s_CameraFront * moveSpeed * Window::deltatime();
	}
	if (Input::isKeyDown(Keys::D)) {
		s_CameraPos -= s_CameraFront.cross(ls_CameraUp).normalise() * moveSpeed * Window::deltatime();
	}
	if (Input::isKeyDown(Keys::A)) {
		s_CameraPos += s_CameraFront.cross(ls_CameraUp).normalise() * moveSpeed * Window::deltatime();
	}

	return Maths::lookAt(s_CameraPos, s_CameraPos + s_CameraFront, ls_CameraUp);
}

Mat4x4 orbitUpdate()
{
	static Vec3 ls_CameraUp = { 0, 1, 0 };

	//static constexpr float SENSITIVITY = 0.3f;

	const auto& pitch = Input::getMousePitch();
	const auto& yaw = Input::getMouseYaw();

	s_CameraPos =
	{
		s_OrbitTarget.x + s_OrbitRadius * sin(pitch) * cos(yaw),
		s_OrbitTarget.y + s_OrbitRadius * cos(pitch),
		s_OrbitTarget.z + s_OrbitRadius * sin(pitch) * sin(yaw),
	};

	return Maths::lookAt(s_CameraPos, s_OrbitTarget, ls_CameraUp);
}