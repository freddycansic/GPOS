#pragma once

#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GENERATE_KEY_INPUT_GETTER_VARIADIC(isKeyPredicate)\
[[nodiscard]] inline bool isKeyPredicate() {};\
\
template<typename... Args>\
[[nodiscard]] bool isKeyPredicate(int first, Args... rest)\
{\
	return isKeyPredicate(first) && isKeyPredicate(rest...);\
}\

struct Vec2;
struct Vec3;

namespace Input
{
	[[nodiscard]] bool isKeyDown(int key);
	GENERATE_KEY_INPUT_GETTER_VARIADIC(isKeyDown);

	[[nodiscard]] bool isKeyJustReleased(int key);
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