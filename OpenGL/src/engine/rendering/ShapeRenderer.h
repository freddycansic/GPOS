#pragma once

#include <array>
#include <string>
#include <memory>

#include "engine/rendering/opengl/Texture.h"
#include "engine/rendering/opengl/VertexArray.h"
#include "engine/rendering/opengl/VertexBuffer.h"
#include "engine/rendering/opengl/IndexBuffer.h"
#include "engine/rendering/opengl/Shader.h"
#include "engine/rendering/Vertex.h"

#include "shapes/Shape.h"

class ShapeRenderer
{

private:
	enum class State {
		UNINITIALISED,
		STOPPED,
		BEGUN,
	};

	const static size_t MAX_VERTICES;
	const static size_t MAX_INDICES;
	
	static State state;

	// vertex + index data
	static std::vector<Vertex> s_VertexBatch;
	static std::vector<unsigned int> s_IndexBatch;

	// keep track of which textures are being used
	static std::array<const Texture*, 32> s_TextureSlots;

	static std::unique_ptr<VertexArray> s_Vao;
	static std::unique_ptr<VertexBuffer> s_Vbo;
	static std::unique_ptr<IndexBuffer> s_Ibo;
	static std::unique_ptr<Shader> s_Shader;

	ShapeRenderer()
	{
	}

	~ShapeRenderer()
	{
	}
	
	static void addShapeIndices(const Shape& shape);
	static void checkBatchBegun();

public:
	/**
	 * Initialises batch rendering, must be called once before the first begin() call.
	 * @brief Initialises batch rendering.
	*/
	static void init();

	/**
	 * Start batch rendering. Must be called after init() has been called once.
	 * @brief Starts batch rendering.
	**/
	static void begin();

	/**
	 * Draw a shape using a solid color.
	 *
	 * @param shape Shape to use.
	 * @param color Color to use as a Vec4.
	**/
	static void draw(Shape& shape, const Vec4& color);

	/**
	 * Draw a shape using a texture.
	 *
	 * @param shape Shape to use.
	 * @param tex Texture to use.
	**/
	static void draw(Shape& shape, const Texture& tex);

	/**
	 * Ends the batch and draws to the screen.
	 * @brief Ends the batch.
	*/
	static void end();
};

