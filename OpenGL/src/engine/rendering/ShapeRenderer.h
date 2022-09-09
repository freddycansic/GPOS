#pragma once

class Texture;
class Shape;
struct Vec4;

namespace ShapeRenderer
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

	/**
	 * Draw a shape using a solid color.
	 *
	 * @param shape Shape to use.
	 * @param color Color to use as a Vec4.
	**/
	void draw(Shape& shape, const Vec4& color);
	void draw(Shape&& shape, const Vec4& color);

	/**
	 * Draw a shape using a texture.
	 *
	 * @param shape Shape to use.
	 * @param tex Texture to use.
	**/
	void draw(Shape& shape, const Texture& tex);
	void draw(Shape&& shape, const Texture& tex);

	/**
	 * Ends the batch and draws to the screen.
	 * @brief Ends the batch.
	*/
	void end();
};