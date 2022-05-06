#include "ShapeRenderer.h"

#include <stdexcept>
#include <algorithm>

#include "engine/Files.h"
#include "engine/rendering/Renderer.h"
#include "engine/Debug.h"

ShapeRenderer::State ShapeRenderer::state = ShapeRenderer::State::UNINITIALISED;

std::vector<TextureData> ShapeRenderer::s_TextureData;

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
	layout.addElement<GLfloat>(3, false);
	layout.addElement<GLfloat>(4, false);
	layout.addElement<GLfloat>(2, false);
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

	// check if texture already in use
	for (unsigned int i = 0; i < s_TextureData.size(); i++) {
		unsigned int textureID = s_TextureData.at(i).ID;
		if (tex.getID() == textureID) {
			textureSlot = textureID;
			break;
		}
	}

	// if not then add it
	if (textureSlot == -1) {
		s_TextureData.emplace_back(tex.getID(), tex.getHandle());
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
	
	// add texture handles to uniform buffer
	std::array<uint64_t, 1024> textureHandles = {0};
	for (unsigned int i = 0; i < s_TextureData.size(); i++) {
		textureHandles[i] = s_TextureData.at(i).handle;
	}
	
	s_Ubo->setSubData(0, sizeof(uint64_t) * textureHandles.size(), textureHandles.data());

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