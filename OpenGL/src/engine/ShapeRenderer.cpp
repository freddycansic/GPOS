#include "ShapeRenderer.h"

#include <stdexcept>
#include <algorithm>

#include "engine/Files.h"
#include "engine/Renderer.h"

// little bit scared these magic numbers will come back to bite me
const size_t ShapeRenderer::MAX_VERTICES = 50000;
const size_t ShapeRenderer::MAX_INDICES = 75000;

bool ShapeRenderer::s_HasBegun = false;
bool ShapeRenderer::s_IsInitialised = false;

void ShapeRenderer::init()
{
	// TODO FIX ME DDDD:
	s_Shader = Shader(Files::internal("shaders/default.vert"), Files::internal("shaders/default.frag"));	
	s_Shader.bind();

	// setup index array for texture slots
	std::array<int, 32> slots = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	s_Shader.setUniform1iv("u_Textures", slots.size(), slots.data());

	// init vao, allocate empty vbo + ibo
	s_Vbo = VertexBuffer(nullptr, MAX_VERTICES * sizeof(Vertex));
	s_Ibo = IndexBuffer(nullptr, GL_UNSIGNED_INT, MAX_INDICES);
	
	VertexBufferLayout layout;
	layout.addElement<GLfloat>(3, false);
	layout.addElement<GLfloat>(2, false);
	layout.addElement<GLfloat>(1, false);

	s_Vao.addBuffer(s_Vbo, layout);
	s_Vao.bind();
	s_Vbo.bind();
	s_Ibo.bind();
}

void ShapeRenderer::begin()
{
	if (!s_IsInitialised) {
		throw std::runtime_error("ShapeRenderer not initialised, did you call ShapeRenderer::init()?");
	}

	s_HasBegun = true;
}

void ShapeRenderer::checkBatchBegun() {
	if (!s_HasBegun) {
		throw std::runtime_error("ShapeRenderer batch not begun, did you call ShapeRenderer::begin()?");
	}
}

void ShapeRenderer::addShapeIndices(const Shape& shape) {
	// add indices to index buffer
	// dereference to get value at iterator in vector
	unsigned int maxIndex = *std::max_element(s_IndexBatch.begin(), s_IndexBatch.end());
	for (unsigned int index : shape.getIndices()) {
		s_IndexBatch.push_back(index + maxIndex + 1);
	}
}

void ShapeRenderer::draw(Shape& shape, const Vec4& color)
{
	checkBatchBegun();
	addShapeIndices(shape);

	for (auto& vertex : shape.getVertices()) {
		vertex.color = color;
		s_VertexBatch.push_back(vertex);
	}
}

void ShapeRenderer::draw(Shape& shape, const Texture& tex)
{
	checkBatchBegun();
	addShapeIndices(shape);

	for (auto& vertex : shape.getVertices()) {
		//vertex.texID = s_TextureSlots.at;
		s_VertexBatch.push_back(vertex);
	}
}

void ShapeRenderer::end()
{
	if (!s_HasBegun) {
		throw std::runtime_error("ShapeRenderer batch not begun, did you call ShapeRenderer::begin()?");
	}

	s_Vbo.bind();
	s_Vbo.setSubData(0, sizeof(Vertex) * s_VertexBatch.size(), s_VertexBatch.data());
	
	Renderer::draw(s_Vao, s_Ibo, s_Shader);

	s_HasBegun = false;
}
