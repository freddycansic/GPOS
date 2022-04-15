#include "ShapeRenderer.h"

#include <stdexcept>
#include <algorithm>

#include "engine/Files.h"
#include "engine/rendering/Renderer.h"
#include "engine/Debug.h"

// little bit scared these magic numbers will come back to bite me
const size_t ShapeRenderer::MAX_VERTICES = 50000;
const size_t ShapeRenderer::MAX_INDICES = 75000;

bool ShapeRenderer::s_HasBegun = false;
bool ShapeRenderer::s_IsInitialised = false;

std::array<unsigned int, 32> ShapeRenderer::s_TextureSlots;

std::vector<Vertex> ShapeRenderer::s_VertexBatch;
std::vector<unsigned int> ShapeRenderer::s_IndexBatch;

std::shared_ptr<VertexArray> ShapeRenderer::s_Vao = nullptr;
std::shared_ptr<VertexBuffer> ShapeRenderer::s_Vbo = nullptr;
std::shared_ptr<IndexBuffer> ShapeRenderer::s_Ibo = nullptr;
std::shared_ptr<Shader> ShapeRenderer::s_Shader = nullptr;

void ShapeRenderer::init()
{
	// TODO FIX ME DDDD:
	s_Shader = std::make_shared<Shader>(Files::internal("shaders/default.vert"), Files::internal("shaders/default.frag"));
	s_Shader->bind();

	// setup index array for texture slots
	std::array<int, 32> slots = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	s_Shader->setUniform1iv("u_Textures", slots.size(), slots.data());

	// init vao, allocate empty vbo + ibo
	s_Vao = std::make_shared<VertexArray>();
	s_Vbo = std::make_shared<VertexBuffer>(nullptr, MAX_VERTICES * sizeof(Vertex));
	s_Ibo = std::make_shared<IndexBuffer>(nullptr, GL_UNSIGNED_INT, MAX_INDICES);
	
	VertexBufferLayout layout;
	layout.addElement<GLfloat>(3, false);
	layout.addElement<GLfloat>(4, false);
	layout.addElement<GLfloat>(2, false);
	layout.addElement<GLfloat>(1, false);

	s_Vao->addBuffer(*s_Vbo, layout);
	s_Vbo->unbind();
	s_Ibo->unbind();

	s_IsInitialised = true;
}

void ShapeRenderer::begin()
{
	if (!s_IsInitialised) {
		throw std::runtime_error("ShapeRenderer not initialised, did you call ShapeRenderer::init()?");
	}

	s_HasBegun = true;
}

void ShapeRenderer::draw(const Shape& shape, const Vec4& color)
{
	checkBatchBegun();
	addShapeIndices(shape);

	// copy vertices
	auto vertices = shape.getVertices();

	// modify vertices and add to buffer
	for (auto& vertex : vertices) {
		vertex.texID = -1;
		vertex.color = color;
		s_VertexBatch.push_back(vertex);
	}
}

void ShapeRenderer::draw(const Shape& shape, const Texture& tex)
{
	checkBatchBegun();
	addShapeIndices(shape);

	unsigned int textureSlot;

	// check if texture already has a slot 
	auto textureSlotItr = std::find(s_TextureSlots.begin(), s_TextureSlots.end(), tex.getID());
	
	// if the returned iterator doesnt point past the end of the array = if it found it
	if (textureSlotItr != std::end(s_TextureSlots)) {
		textureSlot = std::distance(s_TextureSlots.begin(), textureSlotItr); // return position of slot
	}
	
	else {
		// if not then check if there is space for another texture = check for a space with default texture
		auto emptySlotItr = std::find(s_TextureSlots.begin(), s_TextureSlots.end(), 0);

		// if so then insert the texture
		if (emptySlotItr != std::end(s_TextureSlots)) {
			
			textureSlot = std::distance(s_TextureSlots.begin(), emptySlotItr);
			
			s_TextureSlots[textureSlot] = tex.getID();
		}
		else {
			throw std::runtime_error("No texture slots left!"); // ill cross this bridge when i come to it
		}
	}

	// copy vertices
	auto vertices = shape.getVertices();

	for (auto& vertex : vertices) {
		vertex.texID = (float) textureSlot;
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

	for (size_t i = 0; i < s_TextureSlots.size(); i++) {
		const auto& texID = s_TextureSlots[i];

		if (texID == 0) continue;

		GLAPI(glActiveTexture(GL_TEXTURE0 + i));
		GLAPI(glBindTexture(GL_TEXTURE_2D, texID));
		//texture.bindToSlot(i);
	}

	Renderer::draw(*s_Vao, *s_Ibo, *s_Shader);

	// clear buffers
	s_VertexBatch.clear();
	s_IndexBatch.clear();

	s_HasBegun = false;
}

void ShapeRenderer::checkBatchBegun() {
	if (!s_HasBegun) {
		throw std::runtime_error("ShapeRenderer batch not begun, did you call ShapeRenderer::begin()?");
	}
}

void ShapeRenderer::addShapeIndices(const Shape& shape) {
	// add indices to index buffer

	// find maxIndex to offset next indices so they dont reference any previous ones
	unsigned int maxIndex;
	if (s_IndexBatch.size() == 0) {
		maxIndex = 0;
	}
	else {
		// dereference to get value at iterator in vector
		maxIndex = *std::max_element(s_IndexBatch.begin(), s_IndexBatch.end()) + 1;
	}

	for (unsigned int index : shape.getIndices()) {
		s_IndexBatch.push_back(index + maxIndex);
	}
}