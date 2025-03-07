#include "TitleScene.h"
#include "GameScene.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleCursor.h"
#include "ModuleTime.h"
#include "ModuleAudio.h"
#include "AudioContainer.h"
#include "ModuleAssetDatabase.h"

#include "FadeCG.h"
#include "TitleMenuCG.h"
#include "SettingsCG.h"

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

    settings_canvas = new SettingsCG();
    settings_canvas->visible = false;

    canvas = new TitleMenuCG(*settings_canvas);

    Engine::Instance().m_render->AddToRenderQueue(*this);

    Engine::Instance().m_cursor->AddCursor("hand_cursor", Engine::Instance().m_assetsDB->GetTexture("mouse_cursor3"), { 0,0,23,23 }, { -2,-2 }, 2);
    Engine::Instance().m_cursor->AddDefaultCursor(Engine::Instance().m_assetsDB->GetTexture("mouse_cursor2"), { 0,0,23,23 }, { -2,-3 }, 2);
    Engine::Instance().m_cursor->SelectDefaultCursor();


    mt19937 engine(std::random_device{}());
    randomSoundTime = uniform_real_distribution<float>(minRandomSoundTime, maxRandomSoundTime)(engine);

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


    if (settings_canvas->visible || fade->IsFading()) {
        if (canvas->IsInteractable())
            canvas->SetInteractable(false);
    }
    else {
        if (!canvas->IsInteractable())
            canvas->SetInteractable(true);
    }

    canvas->UpdateCanvas();
    settings_canvas->UpdateCanvas();
    fade->UpdateCanvas();
    if (!fade->IsFading() && starting_game) {
       /// Pass to game if needs to be a newGame
       Engine::Instance().s_game->Activate();
       Desactivate();
    }
    return true;
}

bool TitleScene::PostUpdate()
{
    return true;
}

void TitleScene::Render()
{
    canvas->RenderCanvas();
    settings_canvas->RenderCanvas();
    fade->RenderCanvas();
}

bool TitleScene::CleanUp()
{
    delete canvas;
    delete fade;
    delete settings_canvas;
    Engine::Instance().m_render->RemoveFomRenderQueue(*this);
    return true;
}
