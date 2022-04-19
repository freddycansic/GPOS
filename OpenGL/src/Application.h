#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "engine/rendering/opengl/Texture.h"
#include "engine/rendering/shapes/Rectangle.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/Window.h"
#include "engine/Files.h"

#include "maths/Mat4.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class Application
{
private:
	// i really dont like this
	std::unique_ptr<Texture> tex1;
	Mat4 proj;

	float xTranslate = 0.0f, yTranslate = 0.0f, zTranslate = 0.0f;

public:
	Application() = default;

	void init();
	void render();
	void imGuiRender();
	void destroy();

	// bad
	float windowWidth = 0.0f, windowHeight = 0.0f;
};