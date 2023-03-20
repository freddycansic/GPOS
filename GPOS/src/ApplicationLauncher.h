#pragma once

#include <GL/glew.h>

#include "Application.h"
#include "engine/Window.h"

struct ApplicationConfig {
	WindowConfig window;
	char* projectDir;
};

class ApplicationLauncher
{
public:
	ApplicationLauncher(Application& app, const ApplicationConfig& config);
};