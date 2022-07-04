#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Vec2;
struct Vec3;

namespace Input
{
	[[nodiscard]] bool isKeyDown(int key);
	[[nodiscard]] bool isKeyJustReleased(int key);

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