#include "Input.h"

#include <algorithm>

#include "Window.h"
#include "maths/Maths.h"
#include "maths/Vectors.h"

namespace Input
{
	bool isKeyDown(int key)
	{
		return glfwGetKey(Window::GLFWWindow(), key) == GLFW_PRESS;
	}

	// initial mouse pos = middle of the screen
	float lastX = static_cast<float>(Window::width()) / 2.0f;
	float lastY = static_cast<float>(Window::height()) / 2.0f;

	float xOffset, yOffset, yaw = -90.0f, pitch = 0.0f;
	Vec3 cameraDirection;

	constexpr float sens = 0.1f;

	void GLAPIENTRY Callbacks::mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		const auto xPos = static_cast<float>(xpos);
		const auto yPos = static_cast<float>(ypos);
		
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

}
