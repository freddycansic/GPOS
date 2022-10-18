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
}

namespace ObjectRenderer
{
	/**
	 * Initialises batch rendering, must be called once before the first begin() call.
	 * @brief Initialises batch rendering.
	**/
	void init();

	/**
	 * Start batch rendering. Must be called after init() has been called once.
	 * @brief Starts batch rendering.
	**/
	void begin();
	
	void draw(Object& object, RenderingFlag flags = 0);
	void draw(Object&& object, RenderingFlag flags = 0);
	void draw(const Light& light);

	/**
	 * Ends the batch and draws to the screen.
	 * @brief Ends the batch.
	*/
	void end();
};