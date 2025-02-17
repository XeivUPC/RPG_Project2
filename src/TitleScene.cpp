#include "TitleScene.h"
#include "GameScene.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleCursor.h"
#include "ModuleAssetDatabase.h"

#include "FadeCG.h"

TitleScene::TitleScene(bool start_active) : ModuleScene(start_active)
{
}

TitleScene::~TitleScene()
{
}

bool TitleScene::Init()
{
    return true;
}

bool TitleScene::Start()
{
    fade = new FadeCG(0, 0, 36);
    fade->FadeOut(1);
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
    fade->UpdateCanvas();
    if (!fade->IsFading()) {
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
    fade->RenderCanvas();
}

bool TitleScene::CleanUp()
{
    delete fade;
    delete canvas;
    Engine::Instance().m_render->RemoveFomRenderQueue(*this);
    return true;
}
