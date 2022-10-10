#include "ApplicationLauncher.h"
#include "Application.h"

int main(int argc, char* argv[])
{
	ApplicationConfig config;
	
	config.window.title = "Hi";
	config.window.maximised = true;
	config.window.vsync = false;
	config.window.resizable = true;
	config.projectDir = (argc > 1) ? argv[1] : nullptr;

	Application app;
	ApplicationLauncher appHandler(app, config);

	return 0;
}