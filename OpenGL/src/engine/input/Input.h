#pragma once

#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Vec2;
struct Vec3;
class Keybind;

namespace Input
{
	Keybind getFunctionKeybind(void(*function)());

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