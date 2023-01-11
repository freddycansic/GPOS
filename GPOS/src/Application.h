#pragma once

#include "engine/rendering/opengl/Texture.h"

class Application
{
private:
	Texture tex1, tex2;

	char* openedProject = nullptr;

public:
	Application() = default;

	void init(char* projectDir = nullptr);
	void render();
	void imGuiRender();
};