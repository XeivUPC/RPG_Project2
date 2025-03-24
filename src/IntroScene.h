#pragma once
#include "ModuleScene.h"
#include "IRendereable.h"

#include "ChronoTimer.h"
#include "SystemEvent.h"


class UICanvas;
class FadeCG;

class IntroScene : public ModuleScene {
	friend class Engine;
public:
	IntroScene(bool start_active = true);
	~IntroScene();

public:

private:
	// Inherited via IInitializable
	bool Init() override;
	// Inherited via IInitializable
	bool Start() override;
	// Inherited via IUpdateable
	bool PreUpdate() override;
	// Inherited via IUpdateable
	bool Update() override;
	// Inherited via IUpdateable
	bool PostUpdate() override;
	// Inherited via ICleanable
	bool CleanUp() override;

private:
	UICanvas* canvas = nullptr;
	FadeCG* fade_bg = nullptr;
	FadeCG* fade_logo = nullptr;

	ChronoTimer timer;
	ChronoTimer logoTimer;

	float bgFadeIn = 2;

	float logoDelay=2;
	float logoFadeIn = 1.5f;
	float logoHold=1;
	float logoFadeOut = 2;
	

	float timeToLoad = 7;

	SystemEvent<int> event1;
	SystemEvent<string> event2;
	SystemEvent<> event3;
	SystemEvent<int, string> event4;
};
