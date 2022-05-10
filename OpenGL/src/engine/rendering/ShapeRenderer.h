#pragma once

#include <memory>

#include "engine/rendering/opengl/Texture.h"
#include "engine/rendering/opengl/VertexArray.h"
#include "engine/rendering/opengl/VertexBuffer.h"
#include "engine/rendering/opengl/IndexBuffer.h"
#include "engine/rendering/opengl/UniformBuffer.h"
#include "engine/rendering/opengl/Shader.h"
#include "engine/rendering/Vertex.h"

#include "shapes/Shape.h"

struct TextureData {
	unsigned int ID = 0;
	uint64_t handle = 0;
};

class ShapeRenderer
{

private:
	enum class State {
		UNINITIALISED,
		STOPPED,
		BEGUN,
	};

	// little bit scared these magic numbers will come back to bite me
	constexpr static size_t MAX_VERTICES = 50000;
	constexpr static size_t MAX_INDICES = 75000;
	constexpr static size_t MAX_TEXTURES = 1024;
	
	static State state;

	// vertex + index data
	static std::vector<Vertex> s_VertexBatch;
	static std::vector<unsigned int> s_IndexBatch;

	// keep track of which textures are being used
	static std::vector<TextureData> s_TextureData;

	static std::unique_ptr<VertexArray> s_Vao;
	static std::unique_ptr<VertexBuffer> s_Vbo;
	static std::unique_ptr<IndexBuffer> s_Ibo;
	static std::unique_ptr<UniformBuffer> s_Ubo;
	static std::unique_ptr<Shader> s_Shader;

	ShapeRenderer()
	{
	}

	~ShapeRenderer()
	{
	}
	
	static void addShapeIndices(const Shape& shape);
	static void checkBatchReady();

public:
	/**
	 * Initialises batch rendering, must be called once before the first begin() call.
	 * @brief Initialises batch rendering.
	*/	static void init();

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

