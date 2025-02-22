#pragma once
#include "Module.h"
#include "Vector2Int.h"
#include <string>

#define NUM_MOUSE_BUTTONS 5

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
	friend class Engine;
public:

	ModuleInput(bool start_active = true);
	~ModuleInput();

	// Inherited via IInitializable
	bool Init() override;
	// Inherited via IInitializable
	bool Start() override;
	// Inherited via IUpdateable
	bool PreUpdate() override;
	// Inherited via ICleanable
	bool CleanUp() override;


	KeyState GetKey(int id) const;

	KeyState GetMouseButtonDown(int id) const;

	const string& GetTextInput();

	bool GetWindowEvent(EventWindow ev);

	Vector2Int GetMousePosition();
	Vector2Int GetMouseMotion();

	void ForceQuit();

private:
	bool windowEvents[WE_COUNT];
	KeyState* keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	string textInput;
	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;
};