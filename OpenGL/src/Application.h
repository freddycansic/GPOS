#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "engine/rendering/opengl/Texture.h"
#include "engine/rendering/shapes/Rectangle.h"
#include "engine/rendering/shapes/Cube.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/Window.h"
#include "engine/Files.h"

#include "maths/Mat4.h"
#include "maths/Transform.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class Application
{
private:
	Transform viewTransform;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::string fragmentStringSource, vertexStringSource;

	unsigned int vbo = 0, ibvo = 0, vao = 0, ibo = 0, program = 0;

public:
	Application() = default;

	void init();
	void render();
	void imGuiRender();
	void destroy();

	// bad
	float windowWidth = 0.0f, windowHeight = 0.0f;
};