#pragma once
#include "ModuleScene.h"
#include "StepTimer.h"


class UICanvas;
class FadeCG;

class TitleScene : public ModuleScene{
	friend class Engine;
public:
	TitleScene(bool start_active = true);
	~TitleScene();

public:
	void StartGame(bool _newGame);

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
	UICanvas* settings_canvas = nullptr;
	FadeCG* fade = nullptr;

	bool starting_game = false;
	bool newGame = false;


	StepTimer randomSoundTimer;
	float randomSoundTime = 10;
	float minRandomSoundTime = 5;
	float maxRandomSoundTime = 15;
};
