#include "Input.h"

#include <algorithm>
#include <unordered_map>
#include <array>

#include "engine/Debug.h"
#include "engine/rendering/gui/GUI.h"
#include "Keys.h"
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

	constexpr int JUST_RELEASED = 3;

	std::array<int, MouseButtons::LAST.keyCode + 1> mouseButtonStates;

	void GLAPIENTRY Callbacks::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (mouseButtonStates[button] == GLFW_PRESS && action == GLFW_RELEASE)
		{
			mouseButtonStates[button] = JUST_RELEASED;
			return;
		}

		mouseButtonStates[button] = action;
	}

	bool isMouseButtonDown(const Key& button)
	{
		return mouseButtonStates[button.keyCode] == GLFW_PRESS;
	}

	bool isMouseButtonJustReleased(const Key& button)
	{
		if (mouseButtonStates[button.keyCode] == JUST_RELEASED)
		{
			mouseButtonStates[button.keyCode] = GLFW_RELEASE;
			return true;
		}

		return false;
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

	std::array<int, Keys::LAST.keyCode + 1> keyStates;

	void GLAPIENTRY Callbacks::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key < 0) return;

		if ((keyStates[key] == GLFW_PRESS || keyStates[key] == GLFW_REPEAT) && action == GLFW_RELEASE)
		{
			keyStates[key] = JUST_RELEASED;
			return;
		}

		keyStates[key] = action;
	}

	bool isKeyDown(const Key& key)
	{
		return keyStates[key.keyCode] == GLFW_PRESS || keyStates[key.keyCode] == GLFW_REPEAT;
	}

	bool isKeyJustReleased(const Key& key)
	{
		if (keyStates[key.keyCode] == JUST_RELEASED)
		{
			keyStates[key.keyCode] = GLFW_RELEASE;
			return true;
		}

		return false;
	}

	std::unordered_map<void(*)(), Keybind> keybinds =
	{
#ifdef DEBUG
		{Window::close, {Keys::ESCAPE}},
#endif
		{Window::close, {Keys::LEFT_CONTROL, Keys::Q}},
		{GUI::showNewObjectMenu, {Keys::LEFT_SHIFT, Keys::A}},
		{Scene::setGizmoToTranslate, {Keys::G}},
		{Scene::setGizmoToScale, {Keys::S}},
		{Scene::setGizmoToRotate, {Keys::R}},
		{Scene::duplicateCurrentSelected, {Keys::LEFT_SHIFT, Keys::D}},
		{[]
		{// TODO make mouse buttons be able to activate keybinds
			if (Scene::getSelectedObjects().empty()) return;

			const auto& selectedObjects = Scene::getSelectedObjects();
			const auto& lastSelected = selectedObjects.at(selectedObjects.size() - 1);

			Camera::setOrbitTarget(lastSelected->getAvgPosition());
			Camera::update();

		}, {Keys::F}},

		// TODO TEMP
		//{Window::beginCursorCapture, {Keys::C}},
		//{Window::endCursorCapture, {Keys::V}},
	};

	Keybind getFunctionKeybind(void(*function)())
	{
		return keybinds.at(function);
	}

	bool isKeybindJustReleased(const Keybind& keybind)
	{
		unsigned int numKeysDown = 0, numKeysJustReleased = 0;
		const auto& keys = keybind.getKeys();

		for (const auto& key : keys)
		{
			if (isKeyDown(key))
			{
				++numKeysDown;
				continue;
			}

			if (isKeyJustReleased(key))
			{
				++numKeysJustReleased;
			}
		}

		if (numKeysDown >= keys.size()) return false;

		return numKeysDown + numKeysJustReleased == keys.size();
	}

	void processFunctionKeybindPresses()
	{
		for (const auto& [function, keybind] : keybinds)
		{
			if (isKeybindJustReleased(keybind)) std::invoke(function);
		}
	}
}