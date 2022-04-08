#pragma once

#include <array>
#include <string>

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
	static std::vector<Vertex> s_VertexBatch;
	static std::vector<unsigned int> s_IndexBatch;
	static std::array<const Texture&, 32> s_TextureSlots;

	static VertexArray s_Vao;
	static VertexBuffer s_Vbo;
	static IndexBuffer s_Ibo;
	static Shader s_Shader;

	ShapeRenderer()
	{
	}

	~ShapeRenderer()
	{
	}
	
	static void addShapeIndices(const Shape& shape);
	static void checkBatchBegun();

public:

	static void init();
	static void begin();
	static void draw(Shape& shape, const Vec4& color);
	static void draw(Shape& shape, const Texture& tex);
	static void end();
};

