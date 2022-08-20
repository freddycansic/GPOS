#pragma once

#include <memory>

#include "engine/rendering/opengl/Texture.h"
#include "engine/rendering/shapes/Cube.h"

#include "maths/Transform.h"

class Application
{
private:
	// i really dont like this TODO make better
	std::unique_ptr<Texture> tex1, tex2;

	std::vector<std::pair<std::unique_ptr<Shape>, Vec4>> gameObjects;

	Transform cubeTransform, viewTransform;

public:
	Application() = default;

	void init();
	void render();
	void imGuiRender();
	void destroy();
};