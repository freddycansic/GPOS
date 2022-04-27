#include "ShapeRenderer.h"

#include <stdexcept>
#include <algorithm>

#include "engine/Files.h"
#include "engine/rendering/Renderer.h"
#include "engine/Debug.h"

// little bit scared these magic numbers will come back to bite me
const size_t ShapeRenderer::MAX_VERTICES = 50000;
const size_t ShapeRenderer::MAX_INDICES = 75000;

ShapeRenderer::State ShapeRenderer::state = ShapeRenderer::State::UNINITIALISED;

std::array<const Texture*, 32> ShapeRenderer::s_TextureSlots = {nullptr};

std::vector<Vertex> ShapeRenderer::s_VertexBatch;
std::vector<unsigned int> ShapeRenderer::s_IndexBatch;

std::unique_ptr<VertexArray> ShapeRenderer::s_Vao = nullptr;
std::unique_ptr<VertexBuffer> ShapeRenderer::s_Vbo = nullptr;
std::unique_ptr<IndexBuffer> ShapeRenderer::s_Ibo = nullptr;
std::unique_ptr<Shader> ShapeRenderer::s_Shader = nullptr;

void ShapeRenderer::init()
{
	// TODO FIX ME DDDD:
	s_Shader = std::make_unique<Shader>(Files::internal("shaders/default.vert"), Files::internal("shaders/default.frag"));
	s_Shader->bind();

	// setup index array for texture slots
	std::array<int, 32> slots = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	s_Shader->setUniform1iv("u_Textures", slots.size(), slots.data());

	// init vao, allocate empty vbo + ibo
	s_Vao = std::make_unique<VertexArray>();
	s_Vbo = std::make_unique<VertexBuffer>(nullptr, MAX_VERTICES * sizeof(Vertex));
	s_Ibo = std::make_unique<IndexBuffer>(nullptr, GL_UNSIGNED_INT, MAX_INDICES);

	VertexBufferLayout layout;
	layout.addElement<GLfloat>(3, false);
	layout.addElement<GLfloat>(4, false);
	layout.addElement<GLfloat>(2, false);
	layout.addElement<GLfloat>(1, false);

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
	checkBatchBegun();
	addShapeIndices(shape);

	shape.recalculateVertices();

	// modify vertices and add to buffer
	for (auto& vertex : shape.getVertices()) {
		vertex.texID = -1;
		vertex.color = color;
	
		s_VertexBatch.push_back(vertex);
	}
}

void ShapeRenderer::draw(Shape& shape, const Texture& tex)
{
	checkBatchBegun();
	addShapeIndices(shape);

	int textureSlot = -1;

	// check if texture already has a slot 
	for (unsigned int i = 0; i < s_TextureSlots.size(); i++) {
		
		const auto& texture = s_TextureSlots[i];
		if (texture == nullptr) continue;
		
		if (tex.getID() == texture->getID()) {
			textureSlot = i;
			break;
		}
	}

	// if it doesn't have a slot already then find a slot with id = 0 = empty and insert it there
	if (textureSlot == -1) {
		for (unsigned int i = 0; i < s_TextureSlots.size(); i++) {
			const auto& texture = s_TextureSlots[i];
			if (texture == nullptr) {
				textureSlot = i;
				s_TextureSlots[i] = &tex;
				break;
			}
		}
	}

	// if the texture has no slot and there is no more available slots then throw exception
	if (textureSlot == -1) {
		throw std::runtime_error("No more available texture slots!");
	}

	shape.recalculateVertices();

	// modify vertices
	for (auto& vertex : shape.getVertices()) {
		vertex.texID = textureSlot;
		s_VertexBatch.push_back(vertex);
	}
}

void ShapeRenderer::end()
{
	checkBatchBegun();

	s_Vao->bind();
	s_Vbo->setSubData(0, sizeof(Vertex) * s_VertexBatch.size(), s_VertexBatch.data());
	s_Ibo->setSubData(0, s_IndexBatch.size(), s_IndexBatch.data());

	s_Shader->bind();
	
	for (unsigned int i = 0; i < s_TextureSlots.size(); i++) {
		const auto& texture = s_TextureSlots[i];

		if (texture == nullptr) continue;

		texture->bindToSlot(i);
	}

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

void ShapeRenderer::checkBatchBegun() {
	if (state == State::STOPPED) {
		throw std::runtime_error("ShapeRenderer batch not begun, did you call ShapeRenderer::begin()?");
	}
}

void ShapeRenderer::addShapeIndices(const Shape& shape) {

	// find maxIndex to offset next indices so they dont reference any previous ones
	unsigned int maxIndex;
	if (s_IndexBatch.size() == 0) {
		maxIndex = 0;
	}
	else {
		// dereference to get value at iterator
		maxIndex = *std::max_element(s_IndexBatch.begin(), s_IndexBatch.end()) + 1;
	}

	for (unsigned int index : shape.getIndices()) {
		s_IndexBatch.push_back(index + maxIndex);
	}
}