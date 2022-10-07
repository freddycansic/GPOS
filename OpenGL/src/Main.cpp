#include "ApplicationLauncher.h"
#include "Application.h"

int main(int argc, char* argv[])
{
	ApplicationConfig config;
	
	config.window.title = "Hi";
	config.window.maximised = true;
	config.window.vsync = false;
	config.window.resizable = true;

	Application app;

	if (argc > 1)
	{
		ApplicationLauncher appHandler(app, config, argv[1]);
	}
	else
	{
		ApplicationLauncher appHandler(app, config);
	}

	return 0;
}