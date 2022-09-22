#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Keys.h"

#define GENERATE_KEY_INPUT_GETTER_VARIADIC(isKeyPredicate)\
template<typename... Args>\
[[nodiscard]] bool isKeyPredicate(const Key& first, const Args&... rest)\
{\
	return isKeyPredicate(first) && isKeyPredicate(rest...);\
}\

struct Vec2;
struct Vec3;

namespace Input
{
	[[nodiscard]] bool isKeyDown(const Key& key);
	GENERATE_KEY_INPUT_GETTER_VARIADIC(isKeyDown);

	[[nodiscard]] bool isKeyJustReleased(const Key& key);
	GENERATE_KEY_INPUT_GETTER_VARIADIC(isKeyJustReleased);

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