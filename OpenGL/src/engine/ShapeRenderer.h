#pragma once

#include <array>
#include <string>
#include <memory>

#include "shapes/Rectangle.h"
#include "engine/Texture.h"
#include "engine/VertexArray.h"
#include "engine/VertexBuffer.h"
#include "engine/IndexBuffer.h"
#include "engine/Vertex.h"
#include "engine/Shader.h"

class ShapeRenderer
{

private:
	const static size_t MAX_VERTICES;
	const static size_t MAX_INDICES;
	
	static bool s_HasBegun, s_IsInitialised;

	// vertex + index data
	static std::vector<Vertex> s_VertexBatch;
	static std::vector<unsigned int> s_IndexBatch;

	// keep track of which textures are being used
	static std::array<Texture, 32> s_TextureSlots;

	static std::shared_ptr<VertexArray> s_Vao;
	static std::shared_ptr<VertexBuffer> s_Vbo;
	static std::shared_ptr<IndexBuffer> s_Ibo;

	ShapeRenderer()
	{
	}

	~ShapeRenderer()
	{
	}
	
	static void addShapeIndices(const Shape& shape);
	static void checkBatchBegun();

public:
	static std::shared_ptr<Shader> s_Shader; // TODO TEMPORARY

	static void init();
	static void begin();
	static void draw(const Shape& shape, const Vec4& color);
	static void draw(const Shape& shape, const Texture& tex);
	static void end();
};

