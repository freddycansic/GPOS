#include "ShapeRenderer.h"

#include <stdexcept>
#include <algorithm>

#include "engine/Files.h"
#include "engine/rendering/Renderer.h"
#include "engine/Debug.h"

ShapeRenderer::State ShapeRenderer::state = State::UNINITIALISED;

std::vector<RenderData> ShapeRenderer::s_RenderDataBatch;

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

	RenderData* colorBatch = nullptr;

	// if color batch data exists use it
	for (auto& renderData : s_RenderDataBatch) {
		if (renderData.texHandle == 0) {
			colorBatch = &renderData;
			break;
		}
	}

	// otherwise make one
	if (colorBatch == nullptr) {
		s_RenderDataBatch.emplace_back(std::vector<Vertex>{}, std::vector<unsigned int>{}, 0);
		colorBatch = &s_RenderDataBatch.at(s_RenderDataBatch.size() - 1);
	}

	// add the indices
	addShapeIndices(colorBatch->indices, shape);

	// recaculate vertex positions using current transformation
	shape.recalculateVertices();

	// modify vertices and add to buffer
	for (auto& vertex : shape.getVertices()) {
		vertex.color = color;
	
		colorBatch->vertices.push_back(vertex);
	}
}

void ShapeRenderer::draw(Shape& shape, const Texture& tex)
{
	checkBatchReady();

	RenderData* textureBatch = nullptr;

	for (auto& renderData : s_RenderDataBatch) {
		if (renderData.texHandle == tex.getHandle()) {
			textureBatch = &renderData;
			break;
		}
	}

	if (textureBatch == nullptr) {
		s_RenderDataBatch.emplace_back(std::vector<Vertex>{}, std::vector<unsigned int>{}, tex.getHandle());
		textureBatch = &s_RenderDataBatch.at(s_RenderDataBatch.size() - 1);
	}

	addShapeIndices(textureBatch->indices, shape);

	shape.recalculateVertices();

	// modify vertices
	for (auto& vertex : shape.getVertices()) {
		textureBatch->vertices.push_back(vertex);
	}
}

void ShapeRenderer::end() {
	checkBatchReady();

	s_Vao->bind();

	for (const auto& [vertices, indices, texHandle] : s_RenderDataBatch) {
		s_Vbo->setSubData(0, sizeof(Vertex) * vertices.size(), vertices.data());
		s_Ibo->setSubData(0, indices.size(), indices.data());

		s_Shader->bind();
		s_Shader->setUniform1ui64("u_TexHandle", texHandle);

		Renderer::draw(*s_Vao, *s_Ibo, *s_Shader);

		//s_Vbo->setSubData(0, sizeof(Vertex) * vertices.size(), nullptr);
		//s_Ibo->setSubData(0, indices.size(), nullptr);
	}

	// clear buffers
	s_RenderDataBatch.clear();
	
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

void ShapeRenderer::addShapeIndices(std::vector<unsigned int>& indexBuffer, const Shape& shape) {

	// find maxIndex to offset next indices so they dont reference any previous ones
	unsigned int maxIndex;
	if (indexBuffer.empty()) {
		maxIndex = 0;
	}
	else {
		// dereference to get value at iterator
		maxIndex = *std::ranges::max_element(indexBuffer.begin(), indexBuffer.end()) + 1;
	}

	for (unsigned int index : shape.getIndices()) {
		indexBuffer.push_back(index + maxIndex);
	}
}