#include "TitleScene.h"
#include "GameScene.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleCursor.h"
#include "ModuleAssetDatabase.h"

#include "FadeCG.h"
#include "TitleMenuCG.h"
#include "SettingsCG.h"

TitleScene::TitleScene(bool start_active) : ModuleScene(start_active)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::StartGame(bool _newGame)
{
    fade->FadeIn(1);
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

    fade = new FadeCG(0, 0, 36);
    fade->FadeOut(1);

    settings_canvas = new SettingsCG();
    settings_canvas->visible = false;

    canvas = new TitleMenuCG(*settings_canvas);

    Engine::Instance().m_render->AddToRenderQueue(*this);

    Engine::Instance().m_cursor->SetCursor(Engine::Instance().m_assetsDB->GetTexture("mouse_cursor"), { 0,0,18,18 }, { 1,1 }, 1);

    return true;
}

bool TitleScene::PreUpdate()
{
    return true;
}

bool TitleScene::Update()
{
    if(!settings_canvas->visible && !fade->IsFading())
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
