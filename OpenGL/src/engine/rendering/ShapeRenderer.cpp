#include "ShapeRenderer.h"

#include <stdexcept>
#include <algorithm>

#include "engine/Files.h"
#include "engine/rendering/Renderer.h"
#include "engine/Debug.h"

ShapeRenderer::State ShapeRenderer::state = ShapeRenderer::State::UNINITIALISED;

std::vector<Vertex> ShapeRenderer::s_VertexBatch;
std::vector<unsigned int> ShapeRenderer::s_IndexBatch;

std::unique_ptr<VertexArray> ShapeRenderer::s_Vao = nullptr;
std::unique_ptr<VertexBuffer> ShapeRenderer::s_Vbo = nullptr;
std::unique_ptr<IndexBuffer> ShapeRenderer::s_Ibo = nullptr;
std::unique_ptr<UniformBuffer> ShapeRenderer::s_Ubo = nullptr;
std::unique_ptr<Shader> ShapeRenderer::s_Shader = nullptr;

void ShapeRenderer::init()
{
	// TODO FIX ME DDDD:
	s_Shader = std::make_unique<Shader>(Files::internal("shaders/default.vert"), Files::internal("shaders/default.frag"));
	s_Shader->bind();

	// init vao, allocate empty vbo + ibo
	s_Vao = std::make_unique<VertexArray>();
	s_Vbo = std::make_unique<VertexBuffer>(nullptr, MAX_VERTICES * sizeof(Vertex));
	s_Ibo = std::make_unique<IndexBuffer>(nullptr, GL_UNSIGNED_INT, MAX_INDICES);
	s_Ubo = std::make_unique<UniformBuffer>(nullptr, MAX_TEXTURES * sizeof(uint64_t));

	VertexBufferLayout layout;
	layout.addElement<float>(3, false);
	layout.addElement<float>(4, false);
	layout.addElement<float>(2, false);
	layout.addElement<uint64_t>(1, false);

	s_Vao->addBuffer(*s_Vbo, layout);
	s_Vbo->unbind();
	s_Ibo->unbind();

	state = State::STOPPED;
}

void ShapeRenderer::begin()
{
	if (state == State::UNINITIALISED) {
		throw std::runtime_error("ShapeRenderer not initialised, did you call ShapeRenderer::init()?");
	}

	state = State::BEGUN;
}

void ShapeRenderer::draw(Shape& shape, const Vec4& color)
{
	checkBatchReady();
	addShapeIndices(shape);

	shape.recalculateVertices();

	// modify vertices and add to buffer
	for (auto& vertex : shape.getVertices()) {
		vertex.texHandle = 0;
		vertex.color = color;
	
		s_VertexBatch.push_back(vertex);
	}
}

void ShapeRenderer::draw(Shape& shape, const Texture& tex)
{
	checkBatchReady();
	addShapeIndices(shape);

	shape.recalculateVertices();

	// modify vertices
	for (auto& vertex : shape.getVertices()) {
		vertex.texHandle = tex.getHandle();
		s_VertexBatch.push_back(vertex);
	}
}

void ShapeRenderer::end()
{
	checkBatchReady();

	s_Vao->bind();
	s_Vbo->setSubData(0, sizeof(Vertex) * s_VertexBatch.size(), s_VertexBatch.data());
	s_Ibo->setSubData(0, s_IndexBatch.size(), s_IndexBatch.data());
	s_Shader->bind();

	Renderer::draw(*s_Vao, *s_Ibo, *s_Shader);

	// clear buffers
	s_VertexBatch.clear();
	s_IndexBatch.clear();

	s_Vao->unbind();
	s_Vbo->unbind();
	s_Ibo->unbind();
	s_Shader->unbind();

	state = State::STOPPED;
}

void ShapeRenderer::checkBatchReady() {

	switch (state) {
	case State::STOPPED:
		throw std::runtime_error("ShapeRenderer batch not begun, did you call ShapeRenderer::begin()?");

	case State::UNINITIALISED:
		throw std::runtime_error("ShapeRenderer not initialised, did you call ShapeRenderer::init()?");

	default:
		break;

	}
}

void ShapeRenderer::addShapeIndices(const Shape& shape) {

	// find maxIndex to offset next indices so they dont reference any previous ones
	unsigned int maxIndex;
	if (s_IndexBatch.empty()) {
		maxIndex = 0;
	}
	else {
		// dereference to get value at iterator
		maxIndex = *std::ranges::max_element(s_IndexBatch.begin(), s_IndexBatch.end()) + 1;
	}

	for (unsigned int index : shape.getIndices()) {
		s_IndexBatch.push_back(index + maxIndex);
	}
}