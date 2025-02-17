#include "IntroScene.h"
#include "TitleScene.h"
#include "Engine.h"
#include "ModuleRender.h"
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

    fade_bg = new FadeCG(0,0,36);
    fade_bg->FadeIn(bgFadeIn);

    fade_logo = new FadeCG(255, 255, 255, { LOGIC_SCREEN_WIDTH / 2, LOGIC_SCREEN_HEIGHT / 2 }, { 38,36 }, {0.5f,0.5f},3, Engine::Instance().m_assetsDB->GetTexture("team_logo"));

    Engine::Instance().m_cursor->HideAllCursors();

    Engine::Instance().m_render->AddToRenderQueue(*this);
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
            fade_logo->FadeOut(logoFadeOut);
        }
        else if (logoTimer.ReadSec() > logoDelay + logoFadeIn) {

        }
        else if (logoTimer.ReadSec() > logoDelay) {
            fade_logo->FadeIn(logoFadeIn);
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

void IntroScene::Render()
{
    fade_bg->RenderCanvas();
    fade_logo->RenderCanvas();
}

bool IntroScene::CleanUp()
{
    delete canvas;
    delete fade_bg;
    delete fade_logo;
    Engine::Instance().m_render->RemoveFomRenderQueue(*this);
    return true;
}
