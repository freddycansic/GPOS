#include "Maths.h"

#include <random>

std::mt19937 mersenne(time(nullptr));

namespace Maths {

	float radians(float degrees) {
		return degrees * static_cast<float>(PI_OVER_ONE_EIGHTY);
	}

	float degrees(float radians) {
		return radians * static_cast<float>(ONE_EIGHTY_OVER_PI);
	}

	float randf(float min, float max) {
		const std::uniform_real_distribution<float> dist(min, max);
		return dist(mersenne);
	}

	int randint(int min, int max) {
		const std::uniform_int_distribution<int> dist(min, max);
		return dist(mersenne);
	}

}