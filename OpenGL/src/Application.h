#pragma once

#include <memory>

#include "engine/rendering/Material.h"
#include "engine/rendering/opengl/Texture.h"
#include "engine/rendering/shapes/Cube.h"

#include "maths/Transform.h"

class Application
{
private:
	Texture tex1, tex2;

	Material mat1;

	std::vector<std::pair<std::unique_ptr<Shape>, Vec4>> gameObjects;

public:
	Application() = default;

	void init();
	void render();
	void imGuiRender();
	void destroy();
};