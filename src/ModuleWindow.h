#pragma once
#include "Module.h" 
#include <string>

class ModuleWindow : public Module {
	friend class Engine;
public:
	ModuleWindow(bool start_active = true);
	~ModuleWindow();

	void SetFullscreen(bool _fullscreen);
	bool IsFullscreen();

	void SetTitle(const string& title);

public:
	SDL_Window* window = nullptr;

private:
	// Inherited via IInitializable
	bool Init() override;
	// Inherited via ICleanable
	bool CleanUp() override;

private:
	bool fullscreen = false;

	bool vSync = false;

	int width = 1280;
	int height = 720;

	string windowTitle ="DEFAULT_NAME";
};