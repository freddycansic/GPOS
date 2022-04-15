#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "engine/rendering/opengl/Texture.h"
#include "engine/rendering/shapes/Rectangle.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/Window.h"
#include "engine/Files.h"

#include "maths/Mat4.h"
#include "maths/Vectors.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class Application
{
private:
	Mat4 proj;

	float xTranslate = 0.0f, yTranslate = 0.0f;

	// TODO fix this
	const float windowWidth = 3 * 1920 / 4;
	const float windowHeight = 3 * 1080 / 4;


public:
	void init();
	void render();
	void imGuiRender();
	void destroy();
};