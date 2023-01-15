#pragma once

class Application
{
public:
	Application() = default;

	void init(const char* projectDir = nullptr);
	void render();
	void imGuiRender();
};