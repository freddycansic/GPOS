#pragma once

namespace Maths {

	constexpr double PI = 3.14159265358979323846;
	constexpr float PI_OVER_ONE_EIGHTY = PI / 180;
	constexpr float ONE_EIGHTY_OVER_PI = 180 / PI;

	float radians(float degrees);
	float degrees(float radians);
}