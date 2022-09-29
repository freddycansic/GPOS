#include "Input.h"

#include <algorithm>
#include <unordered_map>

#include "engine/GUI.h"
#include "Keys.h"
#include "Keybind.h"
#include "engine/Window.h"
#include "maths/Maths.h"
#include "maths/Vectors.h"

namespace Input
{
	constexpr float sens = 0.2f;

	// initial mouse pos = middle of the screen
	float lastX = static_cast<float>(Window::width()) / 2.0f;
	float lastY = static_cast<float>(Window::height()) / 2.0f;
	bool firstMouseMove = true;

	float xOffset, yOffset, yaw = -90.0f, pitch = 0.0f;
	Vec3 cameraDirection;
	float xPos, yPos;

	void GLAPIENTRY Callbacks::mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		xPos = static_cast<float>(xpos);
		yPos = static_cast<float>(ypos);
		
		if (firstMouseMove) {
			lastX = xPos;
			lastY = yPos;
			firstMouseMove = false;
		}

		xOffset = (xPos - lastX) * sens;
		yOffset = (lastY - yPos) * sens; // inverted as highest y pos is bottom of screen

		lastX = xPos;
		lastY = yPos;

		if (!Window::capturingCursor()) return;

		yaw -= xOffset * sens;
		pitch -= yOffset * sens;

		pitch = std::clamp(pitch, -89.0f, 89.0f);

		cameraDirection = Vec3(
			cos(Maths::radians(yaw)) * cos(Maths::radians(pitch)),
			sin(Maths::radians(pitch)),
			sin(Maths::radians(yaw)) * cos(Maths::radians(pitch))
		).normalise();
	}

	float getMouseX() { return xPos; }
	float getMouseY() { return yPos; }
	float getLastMouseOffsetX() { return xOffset; }
	float getLastMouseOffsetY() { return yOffset; }
	Vec2 getLastMouseOffset() { return { xOffset, yOffset }; }
	float getMouseYaw() { return yaw; }
	float getMousePitch() { return pitch; }
	Vec3 getCameraDirection() { return cameraDirection; }

	int keyStates[Keys::LAST.keyCode];
	constexpr int JUST_RELEASED = 3;

	void GLAPIENTRY Callbacks::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
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
		{Window::close, {Keys::LEFT_CONTROL, Keys::Q}},
		{GUI::showNewObjectMenu, {Keys::LEFT_SHIFT, Keys::A}}
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
