#include "Util.h"

#include <algorithm>

#include "maths/Vectors.h"

namespace Util
{
	bool isMouseHoveredWindow(const ImVec2& mousePos, const ImVec2& windowPos, const ImVec2& windowSize)
	{
		return (mousePos.x > windowPos.x && mousePos.x < windowPos.x + windowSize.x && mousePos.y > windowPos.y && mousePos.y < windowPos.y + windowSize.y);
	}

	Cube getAABBFromPoints(const std::vector<Vec3>& points)
	{
		float minX = 0, minY = 0, minZ = 0, maxX = 0, maxY = 0, maxZ = 0;
		Vec3 avg;

		for (const auto& pos : points)
		{
			minX = pos.x < minX ? pos.x : minX;
			minY = pos.y < minY ? pos.y : minY;
			minZ = pos.z < minZ ? pos.z : minZ;
			maxX = pos.x > maxX ? pos.x : maxX;
			maxY = pos.y > maxY ? pos.y : maxY;
			maxZ = pos.z > maxZ ? pos.z : maxZ;

			avg.x += pos.x;
			avg.y += pos.y;
			avg.z += pos.z;
		}

		const auto numPositions = static_cast<float>(points.size());

		avg.x /= numPositions;
		avg.y /= numPositions;
		avg.z /= numPositions;

		Cube AABB(avg, 1.0f);
		AABB.setScale(maxX - minX, maxY - minY, maxZ - minZ);

		AABB.setPositions(AABB.getMesh().recalculatePositions(AABB.getTransformMatrix()));

		return AABB;
	}

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


