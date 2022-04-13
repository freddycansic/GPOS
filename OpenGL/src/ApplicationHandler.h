#pragma once

#include <string>

#include "Application.h"
#include "engine/Window.h"

struct ApplicationConfig {
	WindowConfig window;
};

class ApplicationHandler
{
public:
	ApplicationHandler(Application& app, const ApplicationConfig& config);

};