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

struct RenderData {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	uint64_t texHandle;
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
	static std::vector<RenderData> s_RenderDataBatch;

	static std::unique_ptr<VertexArray> s_Vao;
	static std::unique_ptr<VertexBuffer> s_Vbo;
	static std::unique_ptr<IndexBuffer> s_Ibo;
	static std::unique_ptr<UniformBuffer> s_Ubo;
	static std::unique_ptr<Shader> s_Shader;
	
	static void addShapeIndices(std::vector<unsigned int>& indexBuffer, const Shape& shape);
	static void checkBatchReady();

public:
	ShapeRenderer() = delete;
	~ShapeRenderer() = delete;

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

