#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Vec2;
struct Vec3;

namespace Input
{
	template<typename T, typename... Args>
	[[nodiscard]] bool isKeyDown(const T& first, const Args&... rest)
	{
		return isKeyDown(first) && isKeyDown(rest...);
	}
	[[nodiscard]] bool isKeyDown(unsigned int key);
	[[nodiscard]] bool isKeyDown();

	[[nodiscard]] bool isKeyJustReleased(unsigned int key);

	[[nodiscard]] float getMouseX();
	[[nodiscard]] float getMouseY();
	[[nodiscard]] float getLastMouseOffsetX();
	[[nodiscard]] float getLastMouseOffsetY();
	[[nodiscard]] Vec2 getLastMouseOffset();
	[[nodiscard]] float getMouseYaw();
	[[nodiscard]] float getMousePitch();
	[[nodiscard]] Vec3 getCameraDirection();

	namespace Callbacks
	{
		void GLAPIENTRY mouseCallback(GLFWwindow* window, double xpos, double ypos);

		void GLAPIENTRY keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	}

}