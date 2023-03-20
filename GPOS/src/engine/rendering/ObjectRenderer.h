#pragma once
#include "gui/Gizmo.h"

class Texture;
class Shape;
struct Vec4;
struct Object;

using RenderingFlag = size_t;

struct Light
{
	Vec3 pos = { 0, 0, 0 };
	Vec3 colour = { 0, 0, 0 };
};

namespace Flags
{
	constexpr RenderingFlag NO_DEPTH_TEST	= 1 << 0;
	constexpr RenderingFlag NO_LIGHTING		= 1 << 1;
	constexpr RenderingFlag ALWAYS_SOLID	= 1 << 2;
}

namespace ObjectRenderer
{
	void init();

	void begin();
	
	void draw(Object& object, RenderingFlag flags = 0);
	void draw(const Light& light);

	void end();
};