#include "Maths.h"

namespace Maths {

	float radians(float degrees) {
		return degrees * PI_OVER_ONE_EIGHTY;
	}

	float degrees(float radians) {
		return radians * ONE_EIGHTY_OVER_PI;
	}

}