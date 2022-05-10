#pragma once

#include "Application.h"
#include "engine/Window.h"

struct ApplicationConfig {
	WindowConfig window;
};

class ApplicationLauncher
{
public:
	ApplicationLauncher(Application& app, const ApplicationConfig& config);
};