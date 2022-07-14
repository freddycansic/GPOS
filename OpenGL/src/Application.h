#pragma once

#include <memory>

#include "engine/rendering/opengl/Texture.h"
#include "engine/rendering/shapes/Cube.h"

#include "maths/Mat4.h"
#include "maths/Transform.h"

class Application
{
private:
	// i really dont like this TODO make better
	std::unique_ptr<Texture> tex1, tex2;
	Mat4 proj;
	Cube cube1, cube2, cube3;
	//Rectangle rect1, rect2;

	Transform cubeTransform, viewTransform;

public:
	Application() = default;

	void init();
	void render();
	void imGuiRender();
	void destroy();
};