#include "Engine.h"
#include "Log.h"
#include "Module.h"
#include "ModuleUpdater.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModulePhysics.h"
#include "ModuleTime.h"
#include "ModuleAssetDatabase.h"
#include "ModuleInput.h"
#include "ModuleCursor.h"
#include "ModuleAudio.h"

#include "Globals.h"

#include <sstream>
#include <iomanip>

//// SCENES /////

#include "IntroScene.h"
#include "TitleScene.h"
#include "GameScene.h"

////////////////


double ModuleTime::deltaTime = 0;
double ModuleTime::fixedDeltaTime = 0;
float ModuleTime::timeScale = 1;

Engine::Engine()
{
	m_window = new ModuleWindow();
	m_physics = new ModulePhysics();
	m_input = new ModuleInput();
	m_cursor = new ModuleCursor();
	m_audio = new ModuleAudio();
	m_render = new ModuleRender();
	m_assetsDB = new ModuleAssetDatabase();
	m_updater = new ModuleUpdater();
	m_time = new ModuleTime();

	s_intro = new IntroScene(true);
	s_title = new TitleScene(false);
	s_game = new GameScene(false);



	AddModule(m_window);
	AddModule(m_physics);
	AddModule(m_input);
	AddModule(m_cursor);
	AddModule(m_audio);
	AddModule(m_time);


	//// SCENES /////

	AddModule(s_intro);
	AddModule(s_title);
	AddModule(s_game);

	////////////////

	AddModule(m_updater);
	AddModule(m_render);
	AddModule(m_assetsDB);

}

Engine& Engine::Instance()
{
	static Engine instance;
	return instance;
}

bool Engine::Init()
{
	bool ret = true;
	for (auto it = list_modules.begin(); it != list_modules.end() && ret; ++it)
	{
		Module* module = *it;
		ret = module->Init();
	}

	LOG("Application Start --------------");

	for (auto it = list_modules.begin(); it != list_modules.end() && ret; ++it)
	{
		Module* module = *it;
		if (module->IsActive())
			ret = module->Start();
	}
	return ret;
}

bool Engine::Update()
{
	bool ret = true;

	if (m_input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret)
		ret = m_updater->PreUpdate();
	if (ret)
		ret = m_updater->Update();
	if (ret)
		ret = m_updater->PostUpdate();

	/*for (auto it = list_modules.begin(); it != list_modules.end() && ret == true; ++it)
	{
		Module* module = *it;
		if (module->IsActive())
		{
			ret = module->PreUpdate();
		}
	}
	for (auto it = list_modules.begin(); it != list_modules.end() && ret == true; ++it)
	{
		Module* module = *it;
		if (module->IsActive())
		{
			ret = module->Update();
		}
	}
	for (auto it = list_modules.begin(); it != list_modules.end() && ret == true; ++it)
	{
		Module* module = *it;
		if (module->IsActive())
		{
			ret = module->PostUpdate();
		}
	}*/

	lastSecFrameCount++;
	if (lastSecFrameTime.ReadMSec() > 1000) {
		lastSecFrameTime.StartTimer();
		averageFps = (averageFps + lastSecFrameCount) / 2;
		lastSecFrameCount = 0;

		stringstream ss;
		ss << TITLE << ": Av.FPS: " << fixed << setprecision(2) << averageFps;
		string titleStr = ss.str();
		m_window->SetTitle(titleStr.c_str());
	}
	

	return ret;
}

bool Engine::CleanUp()
{
	sort(list_modules.begin(), list_modules.end(), [](const Module* a, const Module* b) {
		return a->priority_deleting < b->priority_deleting;
		});

	bool ret = true;
	for (auto it = list_modules.rbegin(); it != list_modules.rend() && ret; ++it)
	{
		Module* item = *it;
		if (item->IsActive())
			ret = item->CleanUp();
		delete item;
	}
	list_modules.clear();

	return ret;
}

void Engine::AddModule(Module* module)
{
	list_modules.emplace_back(module);
}
