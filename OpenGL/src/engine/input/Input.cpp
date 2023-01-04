#include "Input.h"

#include <algorithm>
#include <unordered_map>
#include <array>

#include "Buttons.h"
#include "engine/Debug.h"
#include "engine/rendering/gui/GUI.h"
#include "Keybind.h"
#include "engine/Window.h"
#include "engine/rendering/Renderer.h"
#include "maths/Maths.h"
#include "engine/viewport/Scene.h"
#include "engine/viewport/Camera.h"
#include "maths/Vec2.h"
#include "maths/Vec3.h"

namespace Input
{
	// initial mouse pos = middle of the screen
	float xOffset, yOffset, yaw = Maths::PI<float> / 4, pitch = (3 * Maths::PI<float> / 5);
	Vec3 cameraDirection;
	float xPos, yPos;

	void GLAPIENTRY Callbacks::mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		static constexpr float SENSITIVITY = 0.0025f;
		static bool firstMouseMove = true;
		static float lastX = static_cast<float>(Window::width()) / 2.0f;
		static float lastY = static_cast<float>(Window::height()) / 2.0f;

		xPos = static_cast<float>(xpos);
		yPos = static_cast<float>(ypos);

		if (firstMouseMove) {
			lastX = xPos;
			lastY = yPos;
			firstMouseMove = false;
		}

		xOffset = (xPos - lastX) * SENSITIVITY;
		yOffset = (lastY - yPos) * SENSITIVITY; // inverted as highest y pos is bottom of screen

		lastX = xPos;
		lastY = yPos;

		if (!Window::capturingCursor()) return;

		yaw -= xOffset;
		pitch -= yOffset;

		//pitch = std::clamp(pitch, -89.9f, 89.9f);
		pitch = std::clamp(pitch, 0.01f, Maths::PI<float>); // TODO magic values which stop orbit camera from freaking out when looking directly down/up on/to an object
		yaw = std::fmod(yaw, Maths::PI<float> * 2);

		cameraDirection = Vec3(
			cos(yaw) * cos(pitch),
			sin(pitch),
			sin(yaw) * cos(pitch)
		).normalise();
	}

	float getMouseX() { return xPos; }
	float getMouseY() { return yPos; }
	Vec2 getMousePos() { return { xPos, yPos }; }
	float getLastMouseOffsetX() { return xOffset; }
	float getLastMouseOffsetY() { return yOffset; }
	Vec2 getLastMouseOffset() { return { xOffset, yOffset }; }
	float getMouseYaw() { return yaw; }
	float getMousePitch() { return pitch; }
	Vec3 getCameraDirection() { return cameraDirection; }

	void GLAPIENTRY Callbacks::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if ((MouseButton::states[button] == GLFW_PRESS || MouseButton::states[button] == GLFW_REPEAT) && action == GLFW_RELEASE)
		{
			MouseButton::states[button] = JUST_RELEASED;
			return;
		}

		MouseButton::states[button] = action;
	}

	void GLAPIENTRY Callbacks::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		GLAPI(glViewport(0, 0, width, height));
		Window::setWidth(width);
		Window::setHeight(height);
		Renderer::recalculateProjectionMatrices();
	}

	void GLAPIENTRY Callbacks::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Camera::zoom(static_cast<float>(yoffset));
	}

	void GLAPIENTRY Callbacks::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key < 0) return;

		if ((Key::states[key] == GLFW_PRESS || Key::states[key] == GLFW_REPEAT) && action == GLFW_RELEASE)
		{
			Key::states[key] = JUST_RELEASED;
			return;
		}

		Key::states[key] = action;
	}

	std::unordered_map<void(*)(), Keybind> justReleaseKeybinds =
	{
#ifdef DEBUG
		{Window::close, {Keys::ESCAPE}},
#endif
		{Window::close, {Keys::LEFT_CONTROL, Keys::Q}},
		{GUI::showNewObjectMenu, {Keys::LEFT_SHIFT, Keys::A}},
		{[] {Gizmo::setTool(GizmoTool::TRANSLATE); }, {Keys::G}},
		{[] {Gizmo::setTool(GizmoTool::SCALE); }, {Keys::S}},
		{[] {Gizmo::setTool(GizmoTool::ROTATE); }, {Keys::R}},
		{Scene::duplicateCurrentSelected, {Keys::LEFT_SHIFT, Keys::D}},
		{Scene::selectAll, {Keys::A}},
		{Scene::clearSelection, {Keys::LEFT_CONTROL, Keys::D}},
		{Scene::deleteSelected, {Keys::X}},
		{Camera::setOrbitTargetToLastSelected, {Keys::F}},
		{Scene::loadModelIntoScene, {Keys::LEFT_CONTROL, Keys::I}},
	};

	std::unordered_map<void(*)(), Keybind> heldKeybinds =
	{
		{GUI::showLeo, {Keys::L, Keys::E, Keys::O}},// TODO fix not working
	};

	Keybind getFunctionKeybind(void(*function)())
	{
		if (justReleaseKeybinds.contains(function))
		{
			return justReleaseKeybinds.at(function);
		}

		return heldKeybinds.at(function);
	}

	void processFunctionKeybindPresses()
	{
		for (const auto& [function, keybind] : justReleaseKeybinds)
		{
			if (keybind.isJustReleased()) std::invoke(function);
		}

		for (const auto& [function, keybind] : heldKeybinds)
		{
			if (keybind.isHeld()) std::invoke(function);
		}
	}
}