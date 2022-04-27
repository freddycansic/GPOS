#include "ApplicationLauncher.h"
#include "Application.h"

int main()
{
	ApplicationConfig config;
	config.window.width = 1920 * 3 / 4;
	config.window.height = 1080 * 3 / 4;
	config.window.title = "Hi";

	// all variables inside app must have a default constructor or have default values
	Application app;
	ApplicationLauncher appHandler(app, config);
	
	return 0;
}