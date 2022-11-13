#include "Util.h"

#include <algorithm>

#include "maths/Vectors.h"

namespace Util
{
	float rgbToHue(const Vec3& rgb)
	{
		const auto maxComp = std::max({ rgb.r, rgb.g, rgb.b});
		const auto minComp = std::min({ rgb.r, rgb.g, rgb.b});

		const auto diff = maxComp - minComp;

		if (diff == 0) return 0;

		if (maxComp == rgb.r)
		{
			return 60.0f * fmodf((rgb.g - rgb.b) / diff, 6.0f);
		}

		if (maxComp == rgb.g)
		{
			return 60.0f * (((rgb.b - rgb.r) / diff) + 2);
		}

		if (maxComp == rgb.b)
		{
			return 60.0f * (((rgb.r - rgb.g) / diff) + 4);
		}

		return 0;
	}

	float rgbToHue(const Vec4& rgba)
	{
		return rgbToHue(Vec3(rgba));
	}
}


