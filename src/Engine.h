#pragma once
#include <vector>
#include "ChronoTimer.h"

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleCursor;
class ModuleAssetDatabase;
class ModulePhysics;
class ModuleTime;
class ModuleUpdater;
class ModuleAudio;
class ModuleRender;

//// SCENES /////

class IntroScene;
class TitleScene;
class GameScene;

////////////////

using namespace std;

class Engine {
public:

	static Engine& Instance();

	bool Init();
	bool Update();
	bool CleanUp();

public:
	ModuleWindow* m_window = nullptr;
	ModulePhysics* m_physics = nullptr;
	ModuleInput* m_input = nullptr;
	ModuleCursor* m_cursor = nullptr;
	ModuleAssetDatabase* m_assetsDB = nullptr;
	ModuleTime* m_time = nullptr;
	ModuleUpdater* m_updater = nullptr;
	ModuleAudio* m_audio = nullptr;
	ModuleRender* m_render = nullptr;


	//// SCENES /////

	IntroScene* s_intro;
	TitleScene* s_title;
	GameScene* s_game;

	////////////////

private:
	Engine();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	void AddModule(Module* module);
private:
	vector<Module*> list_modules;

	ChronoTimer lastSecFrameTime;
	int lastSecFrameCount = 0;
	float averageFps = 0.0f;
};
