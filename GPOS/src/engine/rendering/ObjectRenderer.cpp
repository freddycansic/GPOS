#include "ObjectRenderer.h"

#include <stdexcept>
#include <memory>
#include <vector>
#include <map>
#include <future>

#include "Renderer.h"
#include "engine/Colours.h"
#include "engine/rendering/opengl/IndexBuffer.h"
#include "engine/rendering/opengl/VertexBuffer.h"
#include "engine/rendering/opengl/VertexArray.h"
#include "engine/rendering/opengl/Shader.h"
#include "engine/rendering/objects/Object.h"
#include "engine/input/Files.h"
#include "objects/Vertex.h"
#include "engine/Debug.h"
#include "engine/Stats.h"
#include "engine/Util.h"
#include "engine/viewport/Scene.h"

using namespace Flags;

struct BatchData
{
	std::vector<Object*> objects;
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
constexpr static size_t MAX_COUNT_LIGHTS = 16;

State s_State = State::UNINITIALISED;

using Batches = std::map<std::pair<size_t, RenderingFlag>, BatchData>;
Batches s_ObjectBatches; // TODO make name better
Batches s_DepthTestObjectBatches;
std::vector<Light> s_Lights;

std::unique_ptr<VertexArray> s_Vao = nullptr;
std::unique_ptr<VertexBuffer> s_Vbo = nullptr;
std::unique_ptr<IndexBuffer> s_Ibo = nullptr;
std::unique_ptr<Shader> s_Shader = nullptr;

void checkRendererReady(const State& state);
std::vector<GLuint> getCompiledIndexVector(const BatchData& batchData);
void recalculateAllBatchPositions(const BatchData& batchData);
void renderBatch(const std::pair<size_t, RenderingFlag>& handleAndFlags, const BatchData& batchData);

namespace ObjectRenderer
{
	void init()
	{
		s_Shader = std::make_unique<Shader>(Files::internal("shaders/default.vert"), Files::internal("shaders/default.frag"));
		s_Shader->bind();

		// init vao, allocate empty vbo + ibo
		s_Vao = std::make_unique<VertexArray>();
		s_Vbo = std::make_unique<VertexBuffer>(nullptr, MAX_VERTICES * sizeof(Vertex));
		s_Ibo = std::make_unique<IndexBuffer>(nullptr, GL_UNSIGNED_INT, MAX_INDICES);

		s_Lights.reserve(MAX_COUNT_LIGHTS);

		VertexBufferLayout layout;
		layout.addElement<float>(3, false); // position
		layout.addElement<float>(4, false); // colour
		layout.addElement<float>(2, false); // tex coords
		layout.addElement<float>(3, false); // normals

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

	void draw(Object& object, RenderingFlag flags)
	{
		checkRendererReady(s_State);

		size_t handle = 0;
		if (!object.material.texturePath.empty())
		{
			handle = Scene::getTexture(object.material.texturePath).getHandle();
		}

		BatchData* batch = nullptr;
		if (flags & NO_DEPTH_TEST)
		{
			batch = &s_DepthTestObjectBatches[std::make_pair(handle, flags)];
		}
		else
		{
			batch = &s_ObjectBatches[std::make_pair(handle, flags)];
		}


		batch->objects.emplace_back(&object);

		const auto& mesh = object.getMesh();

		batch->indicesCount += mesh.indices.size();
		batch->verticesCount += mesh.positions.size();
	}

	void draw(const Light& light)
	{
		s_Lights.push_back(light);

		ASSERT_WITH_MSG(s_Lights.size() <= MAX_COUNT_LIGHTS, "Maximum number of lights exceeded.");
	}

	void end()
	{
		checkRendererReady(s_State);

		s_Vao->bind();
		s_Shader->bind();

		for (size_t i = 0; i < s_Lights.size(); ++i)
		{
			const auto& [pos, colour] = s_Lights.at(i);
			s_Shader->setUniform3f("u_Lights[" + std::to_string(i) + "].pos", pos);
			s_Shader->setUniform3f("u_Lights[" + std::to_string(i) + "].colour", colour);
		}

		for (auto& [handleAndFlags, batchData] : s_ObjectBatches)
		{
			renderBatch(handleAndFlags, batchData);
		}

		// render batches without depth testing last so they appear in front of everything else
		for (auto& [handleAndFlags, batchData] : s_DepthTestObjectBatches)
		{
			renderBatch(handleAndFlags, batchData);
		}

		// clear buffers
		s_ObjectBatches.clear();
		s_DepthTestObjectBatches.clear();
		s_Lights.clear();

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

void renderBatch(const std::pair<size_t, RenderingFlag>& handleAndFlags, const BatchData& batchData)
{
	const auto& [handle, flags] = handleAndFlags;

	auto f_BatchIndices = std::async(std::launch::async, getCompiledIndexVector, std::ref(batchData));

	std::thread t_RecalculateAllBatchPositions(recalculateAllBatchPositions, std::ref(batchData));

	for (const auto& object : batchData.objects)
	{
		if (Gizmo::getTool() != GizmoTool::TRANSLATE || object->normals.empty())
		{
			object->normals = object->getMesh().recalculateNormals(object->getCombinedTransformations());
		}
	}

	t_RecalculateAllBatchPositions.join();

	std::vector<Vertex> batchVertices;
	batchVertices.reserve(batchData.verticesCount);

	for (const auto& object : batchData.objects)
	{
		auto& mesh = object->getMesh();

		for (unsigned int i = 0; i < object->positions.size(); ++i)
		{
			batchVertices.emplace_back
			(
				object->positions.at(i),
				object->selected ? Vec4(Colours::SELECTION_ORANGE, 1.0f) : object->material.colour,
				mesh.textureCoordinates.at(i),
				object->normals.at(i)
			);
		}
	}

	Stats::numVerticesPerFrame += batchVertices.size();

	const auto& batchIndices = f_BatchIndices.get();

	s_Vbo->setData(sizeof(Vertex) * batchVertices.size(), batchVertices.data());
	s_Ibo->setData(sizeof(GLuint) * batchIndices.size(), batchIndices.data());

	s_Shader->setUniform1ui64("u_TexHandle", handle);
	s_Shader->setUniform1i("u_NoLighting", flags & NO_LIGHTING ? 1 : 0);

	bool wasWireframe = false;
	if (flags & ALWAYS_SOLID)
	{
		if (Renderer::getRenderMode() == RenderMode::WIREFRAME)
		{
			wasWireframe = true;
			Renderer::setRenderMode(RenderMode::SOLID);
		}
	}

	if (flags & NO_DEPTH_TEST)
	{
		GLAPI(glDisable(GL_DEPTH_TEST));
	}

	Renderer::draw(*s_Vao, *s_Ibo, *s_Shader);

	if (wasWireframe) Renderer::setRenderMode(RenderMode::WIREFRAME);
	if (flags & NO_DEPTH_TEST) GLAPI(glEnable(GL_DEPTH_TEST));
}

std::vector<GLuint> getCompiledIndexVector(const BatchData& batchData)
{
	GLuint maxIndex = 0;
	GLuint lastMaxShapeIndex = 0;

	std::vector<GLuint> batchIndices;
	batchIndices.reserve(batchData.indicesCount);

	for (const auto& object : batchData.objects)
	{
		auto& mesh = object->getMesh();

		const auto currentMaxShapeIndex = mesh.getMaxInt();

		if (batchIndices.empty()) {
			maxIndex = 0;
		}
		else {
			maxIndex += lastMaxShapeIndex + 1;
		}	

		lastMaxShapeIndex = currentMaxShapeIndex;

		for (const auto& index : mesh.indices)
		{
			batchIndices.push_back(index + maxIndex);
		}

	}

	return batchIndices;
}

void recalculateAllBatchPositions(const BatchData& batchData)
{
	for (auto& object : batchData.objects)
	{
		if (object->moved)
		{
			object->positions = object->getMesh().recalculatePositions(object->getTransformMatrix());
		}
	}
}