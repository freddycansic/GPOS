#include "Camera.h"

#include "engine/Window.h"
#include "engine/input/Input.h"
#include "engine/input/Keys.h"
#include "maths/Vectors.h"
#include "engine/rendering/Renderer.h"

Camera::CameraMode s_Mode = Camera::CameraMode::FPS_FLY;
Mat4x4 s_View = Mat4x4::identity();

Vec3 s_CameraPos = { 0.0f, 0.0f, 30.0f };

Mat4x4 fpsFlyUpdate();

namespace Camera
{
	// https://stackoverflow.com/questions/45893277/is-it-possible-get-which-surface-of-cube-will-be-click-in-opengl/45946943#45946943
	Ray perspRayFromCameraScreenPos(const Vec2& screenPos)
	{
		const auto w = static_cast<float>(Window::width());
		const auto h = static_cast<float>(Window::height());

		// x and y in range -1 to 1
		const Vec2 NDCPos =
		{
			(2.0f * screenPos.x / w - 1.0f),
			1.0f - 2.0f * screenPos.y / h
			//(2 * screenPos.x - 1) * w/h,
			//(1 - 2 * screenPos.y)
		};

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
			break;

		default:
			break;
		}
	}

	void setMode(CameraMode mode)
	{
		s_Mode = mode;
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
	static Vec3 ls_CameraFront = { 0.0f, 0.0f, 1.0f }; // point forward
	static Vec3 ls_CameraUp = { 0.0f, 1.0f, 0.0f };

	constexpr static float ls_SprintSpeed = 2.0f;
	const float moveSpeed = 10.0f * (Input::isKeyDown(Keys::LEFT_SHIFT) ? ls_SprintSpeed : 1.0f);

	ls_CameraFront = Input::getCameraDirection();

	if (Input::isKeyDown(Keys::W)) {
		s_CameraPos += ls_CameraFront * moveSpeed * Window::deltatime();
	}
	if (Input::isKeyDown(Keys::S)) {
		s_CameraPos -= ls_CameraFront * moveSpeed * Window::deltatime();
	}
	if (Input::isKeyDown(Keys::D)) {
		s_CameraPos -= ls_CameraFront.cross(ls_CameraUp).normalise() * moveSpeed * Window::deltatime();
	}
	if (Input::isKeyDown(Keys::A)) {
		s_CameraPos += ls_CameraFront.cross(ls_CameraUp).normalise() * moveSpeed * Window::deltatime();
	}

	return Maths::lookAt(s_CameraPos, s_CameraPos + ls_CameraFront, ls_CameraUp);
}