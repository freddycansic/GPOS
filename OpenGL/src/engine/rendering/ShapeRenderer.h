#pragma once
#include "gui/Gizmo.h"

class Texture;
class Shape;
struct Vec4;
struct Object;

using RenderingFlag = size_t;

namespace ShapeRenderer
{
	constexpr RenderingFlag NO_DEPTH_TEST = 1;

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
	
	void draw(const Object& object, RenderingFlag flags = 0);

	/**
	 * Ends the batch and draws to the screen.
	 * @brief Ends the batch.
	*/
	void end();
};