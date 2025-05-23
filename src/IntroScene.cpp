#include "IntroScene.h"
#include "TitleScene.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleUpdater.h"
#include "ModuleAudio.h"
#include "ModuleCursor.h"
#include "ModuleAssetDatabase.h"

#include "FadeCG.h"

IntroScene::IntroScene(bool start_active) : ModuleScene(start_active)
{

}

IntroScene::~IntroScene()
{
}

bool IntroScene::Init()
{
    return true;
}

bool IntroScene::Start()
{
    timer.StartTimer();
    logoTimer.StartTimer();

    fade_bg = new FadeCG(33, 25, 17, 0);
    fade_bg->FadeTo(bgFadeIn,255);
    fade_bg->renderLayer = 6;

    //fade_logo = new FadeCG(255, 255, 255, 0, Engine::Instance().m_assetsDB->GetTexture("game_title"), { LOGIC_SCREEN_WIDTH / 2, LOGIC_SCREEN_HEIGHT / 2 }, { 38,36 }, { 0.5f,0.5f }, 0.3f);
    fade_logo = new FadeCG(255, 255, 255,0, Engine::Instance().m_assetsDB->GetTexture("game_title_&_logo"), { LOGIC_SCREEN_WIDTH / 2, LOGIC_SCREEN_HEIGHT / 2 }, { 38,36 }, {0.5f,0.5f},1.5);
    //fade_logo = new FadeCG(255, 255, 255,0, Engine::Instance().m_assetsDB->GetTexture("team_logo"), { LOGIC_SCREEN_WIDTH / 2, LOGIC_SCREEN_HEIGHT / 2 }, { 38,36 }, {0.5f,0.5f},5);
    fade_logo->renderLayer = 7;

    Engine::Instance().m_cursor->HideAllCursors();

    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);

    return true;
}

bool IntroScene::PreUpdate()
{
    return true;
}

bool IntroScene::Update()
{
    fade_bg->UpdateCanvas();
    fade_logo->UpdateCanvas();

    if (!fade_logo->IsFading()) {
        if (logoTimer.ReadSec() > logoDelay + logoFadeIn + logoFadeOut + logoHold) {

        }
        else if (logoTimer.ReadSec() > logoDelay + logoFadeIn + logoHold) {
            fade_logo->FadeTo(logoFadeOut,0);
        }
        else if (logoTimer.ReadSec() > logoDelay + logoFadeIn) {

        }
        else if (logoTimer.ReadSec() > logoDelay) {
            fade_logo->FadeTo(logoFadeIn,255);

            Mix_Chunk* logo_intro = Engine::Instance().m_assetsDB->GetAudio("logo_intro");
            Engine::Instance().m_audio->PlaySFX(logo_intro);
        }
    }
    if (timer.ReadSec() > timeToLoad) {
        Engine::Instance().s_title->Activate();
        Desactivate();
    }
    return true;
}

bool IntroScene::PostUpdate()
{
    return true;
}

bool IntroScene::CleanUp()
{
    delete canvas;
    delete fade_bg;
    delete fade_logo;

    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);
    return true;
}
