#include "Maths.h"

namespace Maths {

	float radians(float degrees) {
		return degrees * static_cast<float>(PI_OVER_ONE_EIGHTY);
	}

	float degrees(float radians) {
		return radians * static_cast<float>(ONE_EIGHTY_OVER_PI);
	}

}