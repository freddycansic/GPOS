#include "ShapeRenderer.h"

#include <stdexcept>
#include <algorithm>
#include <memory>
#include <vector>

#include "Renderer.h"
#include "Vertex.h"
#include "engine/rendering/opengl/IndexBuffer.h"
#include "engine/rendering/opengl/VertexBuffer.h"
#include "engine/rendering/opengl/VertexArray.h"
#include "engine/rendering/opengl/Shader.h"
#include "engine/rendering/opengl/Texture.h"
#include "engine/rendering/shapes/Shape.h"
#include "engine/Files.h"

struct Batch {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	uint64_t texHandle;
};

enum class State {
	UNINITIALISED,
	STOPPED,
	BEGUN,
};

// little bit scared these magic numbers will come back to bite me
constexpr static size_t MAX_VERTICES = 12000000;
constexpr static size_t MAX_INDICES = 20000000; // <-- INDICES LIMIT LIMITS TO ~421875 cubes

State state = State::UNINITIALISED;

std::vector<Batch> s_Batches;

std::unique_ptr<VertexArray> s_Vao = nullptr;
std::unique_ptr<VertexBuffer> s_Vbo = nullptr;
std::unique_ptr<IndexBuffer> s_Ibo = nullptr;
std::unique_ptr<Shader> s_Shader = nullptr;

unsigned int maxIndex = 0;
unsigned int lastMaxShapeIndex = 0;

void addShapeIndices(std::vector<unsigned int>& indexBuffer, const Shape& shape);
void checkBatchReady();

namespace ShapeRenderer {

	void init()
	{
		// TODO FIX ME DDDD:
		s_Shader = std::make_unique<Shader>(Files::internal("shaders/default.vert"), Files::internal("shaders/default.frag"));
		s_Shader->bind();

		// init vao, allocate empty vbo + ibo
		s_Vao = std::make_unique<VertexArray>();
		s_Vbo = std::make_unique<VertexBuffer>(nullptr, MAX_VERTICES * sizeof(Vertex));
		s_Ibo = std::make_unique<IndexBuffer>(nullptr, GL_UNSIGNED_INT, MAX_INDICES);

		VertexBufferLayout layout;
		layout.addElement<float>(3, false);
		layout.addElement<float>(4, false);
		layout.addElement<float>(2, false);

		s_Vao->addBuffer(*s_Vbo, layout);
		s_Vbo->unbind();
		s_Ibo->unbind();

		state = State::STOPPED;
	}

	void begin()
	{
		if (state == State::UNINITIALISED) {
			throw std::runtime_error("ShapeRenderer not initialised, did you call init()?");
		}

		state = State::BEGUN;
	}

	Batch* getBatchFromHandle(std::vector<Batch>& batches, size_t handle)
	{
		Batch* batch;

		// search for colorBatch (texHandle of 0)
		if (const auto& batchIt = std::ranges::find_if(batches.begin(), batches.end(),
			[handle](const auto& renderData) -> bool {
				return renderData.texHandle == handle;
			}

			// if didnt find one make one (iterator at end = not found)
		); batchIt == s_Batches.end()) {
			s_Batches.emplace_back(std::vector<Vertex>{}, std::vector<unsigned int>{}, 0);
			batch = &s_Batches.at(s_Batches.size() - 1);
			// if it did find one then use that
		}
		else {
			batch = & *batchIt;
		}

		return batch;
	}

	// TODO DRY!
	void draw(const Shape& shape, const Vec4& color)
	{
		checkBatchReady();

		Batch* colorBatch = getBatchFromHandle(s_Batches, 0);

		// add the indices
		addShapeIndices(colorBatch->indices, shape);

		// recaculate vertex positions using current transformation
		const auto& newPositions = shape.getMesh().recalculatePositions(shape.getTransformMatrix());

		for (unsigned int i = 0; i < newPositions.size(); ++i)
		{
			colorBatch->vertices.emplace_back
			(
				newPositions.at(i),
				color,
				shape.getMesh().textureCoordinates.at(i)
			);
		}

	}

	void draw(const Shape& shape, const Texture& tex)
	{
		checkBatchReady();

		Batch* textureBatch = getBatchFromHandle(s_Batches, tex.getHandle());

		addShapeIndices(textureBatch->indices, shape);

		const auto& newPositions = shape.getMesh().recalculatePositions(shape.getTransformMatrix());

		for (unsigned int i = 0; i < newPositions.size(); ++i)
		{
			textureBatch->vertices.emplace_back
			(
				newPositions.at(i),
				Vec4(0, 0, 0, 0),
				shape.getMesh().textureCoordinates.at(i)
			);
		}
	}

	void end() {
		checkBatchReady();

		s_Vao->bind();
		s_Shader->bind();

		for (const auto& [vertices, indices, texHandle] : s_Batches) {
			s_Vbo->setSubData(0, sizeof(Vertex) * vertices.size(), vertices.data());
			s_Ibo->setSubData(0, indices.size(), indices.data());

			s_Shader->setUniform1ui64("u_TexHandle", texHandle);

			Renderer::draw(*s_Vao, *s_Ibo, *s_Shader);
		}

		// clear buffers
		s_Batches.clear();

		s_Vao->unbind();
		s_Vbo->unbind();
		s_Ibo->unbind();
		s_Shader->unbind();

		state = State::STOPPED;
	}
}

void checkBatchReady() {

	switch (state) {
	case State::STOPPED:
		throw std::runtime_error("ShapeRenderer batch not begun, did you call begin()?");

	case State::UNINITIALISED:
		throw std::runtime_error("ShapeRenderer not initialised, did you call init()?");

	default:
		break;
	}
}

void addShapeIndices(std::vector<unsigned int>& indexBuffer, const Shape& shape) {

	// find maxIndex to offset next indices so they dont reference any previous ones
	const auto currentMaxShapeIndex = *std::ranges::max_element(shape.getMesh().indices.begin(), shape.getMesh().indices.end());

	if (indexBuffer.empty()) {
		maxIndex = -1;
	}
	else {
		// 0 1 2 3 4,			0 1 2 3 4,			5 4 3 2 1 0
		//						cM = 4, lM = 4,		cM = 5, lM = 4

		// 0 1 2 3 4, 5 6 7 8 9, 15 14 13 12 11 10

		maxIndex += (currentMaxShapeIndex > lastMaxShapeIndex) ? currentMaxShapeIndex : lastMaxShapeIndex + 1;
	}

	lastMaxShapeIndex = currentMaxShapeIndex;

	for (const auto& index : shape.getMesh().indices) {
		indexBuffer.push_back(index + maxIndex + 1);
	}
}