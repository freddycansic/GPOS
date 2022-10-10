#pragma once

#include "engine/rendering/object/shapes/Line.h"
#include "engine/rendering/opengl/Texture.h"

#include "maths/Transform.h"

class Application
{
private:
	Texture tex1, tex2;

	std::vector<std::pair<Line, Vec4>> gameObjects;

	char* openedProject = nullptr;

public:
	Application() = default;

	void init(char* projectDir = nullptr);
	void render();
	void imGuiRender();
	void destroy();
};