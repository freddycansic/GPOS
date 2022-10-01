#include "Camera.h"

#include "Window.h"
#include "maths/Vectors.h"
#include "rendering/Renderer.h"

namespace Camera
{
	// https://stackoverflow.com/questions/45893277/is-it-possible-get-which-surface-of-cube-will-be-click-in-opengl/45946943#45946943
	Vec3 perspRayFromScreenPos(const Vec2& screenPos, const Vec3& rayOrigin)
	{
		const auto w = static_cast<float>(Window::width());
		const auto h = static_cast<float>(Window::height());

		// x and y in range -1 to 1
		const Vec2 NDCPos =
		{
			2.0f * screenPos.x / w - 1.0f,
			1.0f - 2.0f * screenPos.y / h
		};
		
		const Vec3 directionView = Vec3(Vec4(NDCPos.x, NDCPos.y, 0, 1) * Renderer::getProjectionMatrix().adjugateInverse()).normalise();

		const auto invView = Renderer::getViewMatrix().adjugateInverse();

		const Vec3 rayWorldStart = Vec3(Vec4(rayOrigin) * invView);
		const Vec3 rayWorldEnd = Vec3(Vec4(rayOrigin + directionView) * invView);

		return (rayWorldEnd - rayWorldStart).normalise();
	}
}
