#include "TitleScene.h"
#include "GameScene.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleCursor.h"
#include "ModuleInput.h"
#include "ModuleTime.h"
#include "ModuleAudio.h"
#include "AudioContainer.h"
#include "ModuleAssetDatabase.h"
#include "ModuleUpdater.h"
#include "CharacterDatabase.h"
#include "FadeCG.h"
#include "TitleMenuCG.h"
#include "SettingsCG.h"
#include "ParallaxCG.h"
#include "Camera.h"

#include <random>

TitleScene::TitleScene(bool start_active) : ModuleScene(start_active)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::StartGame(bool _newGame)
{
    fade->FadeTo(1,255);
    starting_game = true;
    newGame = _newGame;
}

bool TitleScene::Init()
{
    return true;
}

bool TitleScene::Start()
{
    starting_game = false;

    fade = new FadeCG(33, 25, 17, 255);
    fade->FadeTo(1,0);
    fade->renderLayer = 9;

    parallax = new ParallaxCG();
    parallax->AddLayer(Engine::Instance().m_assetsDB->GetTexture("title_bg3"), 0.0f);
    parallax->AddLayer(Engine::Instance().m_assetsDB->GetTexture("title_bg2"), 0.2f);
    parallax->AddLayer(Engine::Instance().m_assetsDB->GetTexture("title_bg5"), { 0.05f,0 });
    parallax->AddLayer(Engine::Instance().m_assetsDB->GetTexture("title_bg4"), 0.1f);
    parallax->AddLayer(Engine::Instance().m_assetsDB->GetTexture("title_bg1"), 0.3f);
    parallax->AddLayer(Engine::Instance().m_assetsDB->GetTexture("title_bg6"), 0.0f);
    parallax->renderLayer = 5;

    settings_canvas = new SettingsCG(7);
    settings_canvas->isVisible = false;


    canvas = new TitleMenuCG(*settings_canvas);
    canvas->renderLayer = 6;

    Engine::Instance().m_cursor->AddCursor("hand_cursor", Engine::Instance().m_assetsDB->GetTexture("mouse_cursor3"), { 0,0,23,23 }, { -2,-2 }, 1);
    Engine::Instance().m_cursor->AddDefaultCursor(Engine::Instance().m_assetsDB->GetTexture("mouse_cursor2"), { 0,0,23,23 }, { -2,-3 }, 1);
    Engine::Instance().m_cursor->SelectDefaultCursor();
    Engine::Instance().m_cursor->ShowCustomCursor();


    mt19937 engine(std::random_device{}());
    randomSoundTime = uniform_real_distribution<float>(minRandomSoundTime, maxRandomSoundTime)(engine);

    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);

    Engine::Instance().m_audio->PlayMusicAsync(Engine::Instance().m_assetsDB->GetMusic("mainTheme"), 0);

    return true;
}

bool TitleScene::PreUpdate()
{
    return true;
}

bool TitleScene::Update()
{
    randomSoundTimer.Step(ModuleTime::deltaTime);
    if (randomSoundTime < randomSoundTimer.ReadSec()) {

        mt19937 engine(std::random_device{}());
        randomSoundTime = uniform_real_distribution<float>(minRandomSoundTime, maxRandomSoundTime)(engine);
        randomSoundTimer.Start();
        Engine::Instance().m_audio->PlaySFX(Engine::Instance().m_assetsDB->GetAudioContainer("birds_container")->GetNextClip());
    }


    if (settings_canvas->isVisible || fade->IsFading()) {
        if (canvas->IsInteractable())
            canvas->SetInteractable(false);
    }
    else {
        
        if (!canvas->IsInteractable())
            canvas->SetInteractable(true);
    }

    canvas->UpdateCanvas();
   
    if(!settings_canvas->isVisible)
        parallax->UpdatePosition(Engine::Instance().m_input->GetMousePosition() - Engine::Instance().m_render->GetCamera().viewport / 2);

    parallax->UpdateCanvas();
    settings_canvas->UpdateCanvas();
    fade->UpdateCanvas();
    if (!fade->IsFading() && starting_game) {
       /// Pass to game if needs to be a newGame

       if (newGame) {
           CharacterDatabase::Instance().ResetDataToDefault();
           Engine::Instance().s_game->Activate();
           Engine::Instance().s_game->FreshStart();
       }
       else {
           Engine::Instance().s_game->Activate();
           Engine::Instance().s_game->LoadGameSaveData();
       }
      
       Desactivate();
    }
    return true;
}

bool TitleScene::PostUpdate()
{
    return true;
}

bool TitleScene::CleanUp()
{
    delete canvas;
    delete fade;
    delete settings_canvas;

    delete parallax;

    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);
    return true;
}
