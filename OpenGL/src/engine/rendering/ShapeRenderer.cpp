#include "ShapeRenderer.h"

#include <stdexcept>
#include <algorithm>
#include <memory>
#include <vector>
#include <map>

#include "Renderer.h"
#include "Vertex.h"
#include "engine/rendering/opengl/IndexBuffer.h"
#include "engine/rendering/opengl/VertexBuffer.h"
#include "engine/rendering/opengl/VertexArray.h"
#include "engine/rendering/opengl/Shader.h"
#include "engine/rendering/opengl/Texture.h"
#include "engine/rendering/shapes/Shape.h"
#include "engine/Files.h"

struct ShapeData
{
	Shape* shape;
	Vec4 colour;
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

using BatchData = std::map<size_t, std::vector<ShapeData>>;
BatchData shapeBatches; // TODO make name better

std::unique_ptr<VertexArray> s_Vao = nullptr;
std::unique_ptr<VertexBuffer> s_Vbo = nullptr;
std::unique_ptr<IndexBuffer> s_Ibo = nullptr;
std::unique_ptr<Shader> s_Shader = nullptr;

void checkRendererReady(const State& state);
void addShapeToBatches(BatchData& batches, Shape& shape, const Vec4& colour, size_t handle);

namespace ShapeRenderer {

	void init()
	{
		// TODO FIX ME DDDD: = do move semantics for opengl objects
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

	void draw(Shape& shape, const Vec4& color)
	{
		addShapeToBatches(shapeBatches, shape, color, 0);
	}

	void draw(Shape&& shape, const Vec4& color)
	{
		addShapeToBatches(shapeBatches, shape, color, 0);
	}

	void draw(Shape& shape, const Texture& tex)
	{
		addShapeToBatches(shapeBatches, shape, { 0, 0, 0, 0 }, tex.getHandle());
	}

	void draw(Shape&& shape, const Texture& tex)
	{
		addShapeToBatches(shapeBatches, shape, { 0, 0, 0, 0 }, tex.getHandle());
	}
	
	unsigned int maxIndex = 0;
	unsigned int lastMaxShapeIndex = 0;

	void end() {
		checkRendererReady(state);

		s_Vao->bind();
		s_Shader->bind();

		// for every batch
		for (auto& [texHandle, shapeData]: shapeBatches) {

			// tally up number of vertices and indices in batch
			size_t numVerticesInBatch = 0;
			size_t numIndicesInBatch = 0;
			for (const auto& [shape, colour] : shapeData)
			{
				numVerticesInBatch += shape->getMesh().positions.size();
				numIndicesInBatch += shape->getMesh().indices.size();
			}

			//std::vector<Vertex> vertices(numVerticesInBatch);
			//std::vector<unsigned int> indices(numIndicesInBatch);

			std::vector<Vertex> vertices;
			vertices.reserve(numVerticesInBatch);

			std::vector<unsigned int> indices;
			indices.reserve(numIndicesInBatch);

			for (auto& [shape, colour] : shapeData)
			{
				auto& mesh = shape->getMesh();

				// indices
				const auto currentMaxShapeIndex = mesh.getMaxInt();

				if (indices.empty()) {
					maxIndex = -1;
				}
				else {
					// 0 1 2 3 4,			0 1 2 3 4,			5 4 3 2 1 0
					//						cM = 4, lM = 4,		cM = 5, lM = 4

					// 0 1 2 3 4, 5 6 7 8 9, 15 14 13 12 11 10

					maxIndex += (currentMaxShapeIndex > lastMaxShapeIndex) ? currentMaxShapeIndex : lastMaxShapeIndex + 1;
				}

				lastMaxShapeIndex = currentMaxShapeIndex;

				const auto& shapeIndices = mesh.indices;

				for (const auto& index : shapeIndices)
				{
					indices.push_back(index + maxIndex + 1);
				}

				// vertices
				if (shape->moved())
				{
					shape->setPositions(shape->getMesh().recalculatePositions(shape->getTransformMatrix()));
					shape->setMoved(false);
				}

				const auto& newPositions = shape->getPositions();

				for (unsigned int i = 0; i < newPositions.size(); ++i)
				{
					vertices.emplace_back
					(
						newPositions.at(i),
						colour,
						mesh.textureCoordinates.at(i)
					);
				}

				//for (unsigned int i = 0; i < mesh.positions.size(); ++i)
				//{
				//	vertices.emplace_back
				//	(
				//		mesh.positions.at(i),
				//		colour,
				//		mesh.textureCoordinates.at(i)
				//	);
				//}
			}

			s_Vbo->setSubData(0, sizeof(Vertex) * vertices.size(), vertices.data());
			s_Ibo->setSubData(0, indices.size(), indices.data());

			s_Shader->setUniform1ui64("u_TexHandle", texHandle);

			Renderer::draw(*s_Vao, *s_Ibo, *s_Shader);
		}

		// clear buffers
		shapeBatches.clear();

		s_Vao->unbind();
		s_Vbo->unbind();
		s_Ibo->unbind();
		s_Shader->unbind();

		state = State::STOPPED;
	}
}

void checkRendererReady(const State& state) {

	switch (state) {
	case State::STOPPED:
		throw std::runtime_error("ShapeRenderer batch not begun, did you call begin()?");

	case State::UNINITIALISED:
		throw std::runtime_error("ShapeRenderer not initialised, did you call init()?");

	default:
		break;
	}
}

//void addShapeIndices(std::vector<unsigned int>& indexBuffer, const Shape& shape) {
//
//	// find maxIndex to offset next indices so they dont reference any previous ones
//	const auto currentMaxShapeIndex = shape.getMesh().getMaxInt();
//
//	if (indexBuffer.empty()) {
//		maxIndex = -1;
//	}
//	else {
//		// 0 1 2 3 4,			0 1 2 3 4,			5 4 3 2 1 0
//		//						cM = 4, lM = 4,		cM = 5, lM = 4
//
//		// 0 1 2 3 4, 5 6 7 8 9, 15 14 13 12 11 10
//
//		maxIndex += (currentMaxShapeIndex > lastMaxShapeIndex) ? currentMaxShapeIndex : lastMaxShapeIndex + 1;
//	}
//
//	lastMaxShapeIndex = currentMaxShapeIndex;
//
//	for (const auto& index : shape.getMesh().indices) {
//		indexBuffer.push_back(index + maxIndex + 1);
//	}
//}


void addShapeToBatches(BatchData& batches, Shape& shape, const Vec4& colour, size_t handle)
{
	checkRendererReady(state);

	auto& batch = batches[handle];

	batch.emplace_back(&shape, colour);
}