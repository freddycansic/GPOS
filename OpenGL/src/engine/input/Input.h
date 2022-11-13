#pragma once

#include <unordered_map>

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
class Keybind;

namespace Input
{
	[[nodiscard]] bool isKeyDown(const Key& key);
	GENERATE_KEY_INPUT_GETTER_VARIADIC(isKeyDown);

	[[nodiscard]] bool isKeyJustReleased(const Key& key);
	GENERATE_KEY_INPUT_GETTER_VARIADIC(isKeyJustReleased);

	[[nodiscard]] bool isMouseButtonDown(const Key& button);
	[[nodiscard]] bool isMouseButtonJustReleased(const Key& button);

	Keybind getFunctionKeybind(void(*function)());
	bool isKeybindJustReleased(const Keybind& keybind);

	void processFunctionKeybindPresses();

	[[nodiscard]] float getMouseX();
	[[nodiscard]] float getMouseY();
	[[nodiscard]] Vec2 getMousePos();
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
		void GLAPIENTRY mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		void GLAPIENTRY frameBufferSizeCallback(GLFWwindow* window, int width, int height);
		void GLAPIENTRY scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	}

}