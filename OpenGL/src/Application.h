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
	// i really dont like this TODO make better
	std::unique_ptr<Texture> tex1;
	Mat4 proj;
	Cube cube1;

	Transform cubeTransform, viewTransform;

public:
	Application() = default;

	void init();
	void render();
	void imGuiRender();
	void destroy();

	// bad
	float windowWidth = 0.0f, windowHeight = 0.0f;
};