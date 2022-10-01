#pragma once

class Texture;
class Object;
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
	
	void draw(Object& object, const Vec4& color);
	void draw(Object&& object, const Vec4& color);

	void draw(Object& object, const Texture& tex);
	void draw(Object&& object, const Texture& tex);

	/**
	 * Ends the batch and draws to the screen.
	 * @brief Ends the batch.
	*/
	void end();
};