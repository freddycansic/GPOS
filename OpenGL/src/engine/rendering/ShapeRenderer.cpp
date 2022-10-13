#include "ShapeRenderer.h"

#include <stdexcept>
#include <memory>
#include <vector>
#include <map>
#include <future>

#include "Renderer.h"
#include "engine/rendering/opengl/IndexBuffer.h"
#include "engine/rendering/opengl/VertexBuffer.h"
#include "engine/rendering/opengl/VertexArray.h"
#include "engine/rendering/opengl/Shader.h"
#include "engine/rendering/object/shapes/Shape.h"
#include "engine/input/Files.h"
#include "object/Object.h"
#include "object/shapes/Vertex.h"
#include <engine/Debug.h>

struct BatchData
{
	// pointer to constant OBJECT
	std::vector<const Object*> objects;
	size_t indicesCount = 0;
	size_t verticesCount = 0;
};

enum class State {
	UNINITIALISED,
	STOPPED,
	BEGUN,
};

// little bit scared these magic numbers will come back to bite me
constexpr static size_t MAX_VERTICES = 12000000;
constexpr static size_t MAX_INDICES = 20000000; // <-- INDICES LIMIT LIMITS TO ~421875 cubes

State s_State = State::UNINITIALISED;

using Batches = std::map<std::pair<size_t, RenderingFlag>, BatchData>;
Batches s_ObjectBatches; // TODO make name better

std::unique_ptr<VertexArray> s_Vao = nullptr;
std::unique_ptr<VertexBuffer> s_Vbo = nullptr;
std::unique_ptr<IndexBuffer> s_Ibo = nullptr;
std::unique_ptr<Shader> s_Shader = nullptr;

void checkRendererReady(const State& state);
void addObjectToBatches(Batches& batches, const Object& object, RenderingFlag flags);
std::vector<unsigned int> getCompiledIndexVector(const BatchData& batchData);

namespace ShapeRenderer {

	void init()
	{
		// TODO FIX ME DDDD: = do move semantics for opengl Shapes
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

		s_State = State::STOPPED;
	}

	void begin()
	{
		if (s_State == State::UNINITIALISED) 
		{
			throw std::runtime_error("ShapeRenderer not initialised, did you call init()?");
		}

		s_State = State::BEGUN;
	}

	void draw(const Object& object, RenderingFlag flags)
	{
		addObjectToBatches(s_ObjectBatches, object, flags);
	}

	void end()
	{
		checkRendererReady(s_State);

		s_Vao->bind();
		s_Shader->bind();

		// for every batch 
		for (auto& [handleAndFlags, batchData] : s_ObjectBatches) {

			const auto& handle = handleAndFlags.first;
			const auto& flags = handleAndFlags.second;

			auto batchIndicesFuture = std::async(getCompiledIndexVector, std::ref(batchData));

			std::vector<Vertex> batchVertices;
			batchVertices.reserve(batchData.verticesCount);

			for (const auto& object : batchData.objects)
			{
				auto& shape = object->shapePtr;
				auto& mesh = object->shapePtr->getMesh();

				// vertices
				if (shape->moved())
				{
					shape->setPositions(mesh.recalculatePositions(shape->getTransformMatrix()));
					shape->setMoved(false);
				}

				const auto& newPositions = shape->getPositions();
				
				for (unsigned int i = 0; i < newPositions.size(); ++i)
				{
					static const Vec4 orange = { 1, 194.0f/255.0f, 102.0f/255.0f, 1 };

					batchVertices.emplace_back
					(
						newPositions.at(i),
						object->selected ? orange : object->material.colour,
						mesh.textureCoordinates.at(i)
					);
				}
			}

			const auto& batchIndices = batchIndicesFuture.get();

			s_Vbo->setSubData(0, sizeof(Vertex) * batchVertices.size(), batchVertices.data());
			s_Ibo->setSubData(0, batchIndices.size(), batchIndices.data());

			s_Shader->setUniform1ui64("u_TexHandle", handle);

			if (flags & NO_DEPTH_TEST)
			{
				GLAPI(glDisable(GL_DEPTH_TEST));
				Renderer::draw(*s_Vao, *s_Ibo, *s_Shader);
				GLAPI(glEnable(GL_DEPTH_TEST)); // TODO reset filters smth
			} else
			{
				Renderer::draw(*s_Vao, *s_Ibo, *s_Shader);
			}
		}

		// clear buffers
		s_ObjectBatches.clear();

		s_Vao->unbind();
		s_Vbo->unbind();
		s_Ibo->unbind();
		s_Shader->unbind();

		s_State = State::STOPPED;
	}
}

void checkRendererReady(const State& state)
{
	switch (state) {
	case State::BEGUN:
		return;

	case State::STOPPED:
		throw std::runtime_error("ShapeRenderer batch not begun, did you call begin()?");

	case State::UNINITIALISED:
		throw std::runtime_error("ShapeRenderer not initialised, did you call init()?");
	}
}

void addObjectToBatches(Batches& batches, const Object& object, RenderingFlag flags)
{
	checkRendererReady(s_State);

	auto& [batchObjects, indicesCount, verticesCount] = batches[std::make_pair(object.material.texHandle, flags)];

	batchObjects.emplace_back(&object);

	const auto& mesh = object.shapePtr->getMesh();

	indicesCount += mesh.indices.size();
	verticesCount += mesh.positions.size();
}

std::vector<unsigned int> getCompiledIndexVector(const BatchData& batchData)
{
	unsigned int maxIndex = 0;
	unsigned int lastMaxShapeIndex = 0;

	std::vector<unsigned int> batchIndices;
	batchIndices.reserve(batchData.indicesCount);

	for (const auto& object : batchData.objects)
	{
		auto& mesh = object->shapePtr->getMesh();

		// indices
		const auto currentMaxShapeIndex = mesh.getMaxInt();

		if (batchIndices.empty()) {
			maxIndex = 0;
		}
		else {
			// 0 1 2 3 4,			0 1 2 3 4,			5 4 3 2 1 0
			//						cM = 4, lM = 4,		cM = 5, lM = 4

			// m = 4				m = 9				m = 

			// 0 1 2 3 4, 5 6 7 8 9, 15 14 13 12 11 10

			maxIndex += (currentMaxShapeIndex > lastMaxShapeIndex) ? currentMaxShapeIndex - 1 : lastMaxShapeIndex + 1;
		}

		lastMaxShapeIndex = currentMaxShapeIndex;

		for (const auto& index : mesh.indices)
		{
			batchIndices.push_back(index + maxIndex);
		}
	}

	return batchIndices;
}