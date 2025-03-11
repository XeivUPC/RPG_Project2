#include "ModuleInput.h"
#include "Engine.h"
#include "ModuleWindow.h"
#include "ModuleUpdater.h"
#include "Log.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_active) : Module(start_active)
{
    keyboard = new KeyState[MAX_KEYS];
    memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
    memset(mouseButtons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);

	
}

ModuleInput::~ModuleInput()
{
    delete[] keyboard;

	
}

bool ModuleInput::Init()
{
    LOG("Init SDL input event system");
    bool ret = true;
    SDL_Init(0);

    if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
    {
        LOG("SDL_EVENTS could not initialize! SDL_Error: %s", SDL_GetError());
        ret = false;
    }

    return ret;
}

bool ModuleInput::Start()
{
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    SDL_StopTextInput();
    return true;
}

bool ModuleInput::CleanUp()
{
	Engine::Instance().m_updater->RemoveFomUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

bool ModuleInput::PreUpdate()
{
	textInput = "";

	static SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouseButtons[i] == KEY_DOWN)
			mouseButtons[i] = KEY_REPEAT;

		if (mouseButtons[i] == KEY_UP)
			mouseButtons[i] = KEY_IDLE;
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouseButtons[event.button.button - 1] = KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			mouseButtons[event.button.button - 1] = KEY_UP;
			break;

		case SDL_MOUSEMOTION:
			mouseMotionX = event.motion.xrel;
			mouseMotionY = event.motion.yrel;
			mouseX = event.motion.x;
			mouseY = event.motion.y;
			break;

		case SDL_TEXTINPUT:
			textInput = event.text.text;
		}
	}

	if (GetKey(SDL_SCANCODE_ESCAPE))
		windowEvents[WE_QUIT] = true;

	return true;
}

KeyState ModuleInput::GetKey(int id) const
{
	return keyboard[id];
}

KeyState ModuleInput::GetMouseButtonDown(int id) const
{
	return mouseButtons[id - 1];
}

const string& ModuleInput::GetTextInput()
{
	return textInput;
}

bool ModuleInput::GetWindowEvent(EventWindow ev)
{
	return windowEvents[ev];
}

Vector2Int ModuleInput::GetMousePosition()
{
	return Vector2Int(mouseX, mouseY);
}

Vector2Int ModuleInput::GetMouseMotion()
{
	return Vector2Int(mouseMotionX, mouseMotionY);
}

void ModuleInput::ForceQuit()
{
	windowEvents[WE_QUIT] = true;
}
