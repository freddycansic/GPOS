#include "ApplicationLauncher.h"
#include "Application.h"

int main(void)
{
	ApplicationConfig config;
	config.window.width = 3 * 1920 / 4;
	config.window.height = 3 * 1080 / 4;
	config.window.title = "Hi";

	// all variables inside app must have a default constructor or have default values
	Application app;
	ApplicationLauncher appHandler(app, config);
	
	return 0;
}