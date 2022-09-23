#include "ApplicationLauncher.h"
#include "Application.h"

int main(int argc, char* argv[])
{
	ApplicationConfig config;
	//config.window.width = 1920 * 3 / 4;
	//config.window.height = 1080 * 3 / 4;
	config.window.title = "Hi";
	config.window.maxSize = true;
	config.window.vsync = false;

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