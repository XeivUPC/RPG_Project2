#include "ModuleWindow.h"
#include "Log.h"
#include "Globals.h"
#include "UserPrefs.h"

ModuleWindow::ModuleWindow(bool start_active) : Module(start_active)
{
}

ModuleWindow::~ModuleWindow()
{
}

bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s", SDL_GetError());
		ret = false;
	}
	else
	{
		
		width = UserPrefs::Instance().GetInt("screen_width", SCREEN_WIDTH);
		int min_width = MIN_SCREEN_WIDTH;
		height = UserPrefs::Instance().GetInt("screen_height", SCREEN_HEIGHT);
		int min_height = MIN_SCREEN_HEIGHT;

		windowTitle = TITLE;

		// Create window
		Uint32 flags = SDL_WINDOW_SHOWN;

		if (WIN_RESIZABLE)
			flags |= SDL_WINDOW_RESIZABLE;
		if (WIN_FULLSCREEN)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		SDL_SetWindowMinimumSize(window, min_width, min_height);
		SetFullscreen(UserPrefs::Instance().GetBool("screen_fullscreen", false));
		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s", SDL_GetError());
			ret = false;
		}

	}

	return ret;
}

bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");
	
	
	if (!fullscreen) {
		int w = 0;
		int h = 0;
		SDL_GetWindowSize(window, &w, &h);
		UserPrefs::Instance().SaveInt("screen_width", w);
		UserPrefs::Instance().SaveInt("screen_height", h);
	}

	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
	return true;
}

void ModuleWindow::SetFullscreen(bool _fullscreen)
{
	if (_fullscreen) {
		int w = 0;
		int h = 0;
		SDL_GetWindowSize(window, &w, &h);
		UserPrefs::Instance().SaveInt("screen_width", w);
		UserPrefs::Instance().SaveInt("screen_height", h);
	}
	SDL_SetWindowFullscreen(window, _fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	fullscreen = _fullscreen;
	UserPrefs::Instance().SaveBool("screen_fullscreen", fullscreen);
}

bool ModuleWindow::IsFullscreen()
{
	return fullscreen;
}

void ModuleWindow::SetTitle(const string& title)
{
	SDL_SetWindowTitle(window, title.c_str());
}
