#pragma once

#include <GL/glew.h> // TODO fix + DO NOT REMOVE

#include "Application.h"
#include "engine/Window.h"

struct ApplicationConfig {
	WindowConfig window;
};

class ApplicationLauncher
{
public:
	ApplicationLauncher(Application& app, const ApplicationConfig& config, char* projectDir = nullptr);
};