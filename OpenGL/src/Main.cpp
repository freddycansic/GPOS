#include "ApplicationLauncher.h"
#include "Application.h"

int main(int argc, char* argv[])
{
	ApplicationConfig config;
	//config.window.width = 1920 * 3 / 4;
	//config.window.height = 1080 * 3 / 4;
	config.window.title = "Hi";
	config.window.maxSize = true;
	config.window.vsync = true;

	// all variables inside app must have a default constructor or have default values

	if (argc > 2)
	{
		Application app(argv[0]);
		ApplicationLauncher appHandler(app, config);
	} else
	{
		Application app;
		ApplicationLauncher appHandler(app, config);
	}

	return 0;
}