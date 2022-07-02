#include "Input.h"

#include <algorithm>

#include "Keys.h"
#include "Window.h"
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

	void GLAPIENTRY Callbacks::mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		const auto xPos = static_cast<float>(xpos);
		const auto yPos = static_cast<float>(ypos);
		
		if (firstMouseMove) {
			lastX = xPos;
			lastY = yPos;
			firstMouseMove = false;
		}

		xOffset = (xPos - lastX) * sens;
		yOffset = (lastY - yPos) * sens; // inverted as highest y pos is bottom of screen

		lastX = xPos;
		lastY = yPos;

		yaw -= xOffset * sens;
		pitch -= yOffset * sens;

		pitch = std::clamp(pitch, -89.0f, 89.0f);

		cameraDirection = Vec3(
			cos(Maths::radians(yaw)) * cos(Maths::radians(pitch)),
			sin(Maths::radians(pitch)),
			sin(Maths::radians(yaw)) * cos(Maths::radians(pitch))
		).normalise();
	}

	float getLastMouseOffsetX() { return xOffset; }
	float getLastMouseOffsetY() { return yOffset; }
	Vec2 getLastMouseOffset() { return { xOffset, yOffset }; }
	float getMouseYaw() { return yaw; }
	float getMousePitch() { return pitch; }
	Vec3 getCameraDirection() { return cameraDirection; }

	int keyStates[Keys::LAST] = {-1};

	void GLAPIENTRY Callbacks::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		keyStates[key] = action;
	}

	bool isKeyDown(int key)
	{
		return keyStates[key] == GLFW_PRESS;
	}

	bool isKeyJustReleased(int key)
	{
		return keyStates[key] == GLFW_RELEASE;
	}

	void Callbacks::resetKeyEvents()
	{
		for (auto& key : keyStates)
		{
			key = -1;
		}
	}

}
