#pragma once
#include <vector>
#include "ChronoTimer.h"

class Module;
class ModuleAssetDatabase;
class ModuleAudio;
class ModuleCursor;
class ModuleInput;
class ModulePathfinding;
class ModulePhysics;
class ModuleRender;
class ModuleTime;
class ModuleUpdater;
class ModuleWindow;
class ModuleDebug;

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
	ModuleDebug* m_debug = nullptr;


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
