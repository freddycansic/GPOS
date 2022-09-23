#include "Maths.h"

#include <random>
#include <chrono>

std::mt19937 mersenne(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));

namespace Maths {

	float radians(float degrees) {
		return degrees * static_cast<float>(PI_OVER_ONE_EIGHTY);
	}

	float degrees(float radians) {
		return radians * static_cast<float>(ONE_EIGHTY_OVER_PI);
	}

	float randf(float min, float max) {
		std::uniform_real_distribution<float> dist(min, max);
		return dist(mersenne);
	}

	int randint(int min, int max) {
		std::uniform_int_distribution<int> dist(min, max);
		return dist(mersenne);
	}

}