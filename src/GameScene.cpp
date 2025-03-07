#include "GameScene.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleAssetDatabase.h"
#include "ModuleAudio.h"

#include "FadeCG.h"
#include "UITestingCG.h"
#include "UIDialogueBoxCG.h"

GameScene::GameScene(bool start_active) : ModuleScene(start_active)
{
}

GameScene::~GameScene()
{
    
}

bool GameScene::Init()
{
    return true;
}

bool GameScene::Start()
{
    fade = new FadeCG(33, 25, 17, 255);
    fade->FadeTo(1,0);

    canvas = new UITestingCG();
    dialogueCanvas = new UIDialogueBoxCG();
    Engine::Instance().m_render->AddToRenderQueue(*this);

    Engine::Instance().m_audio->PlayMusicAsync(Engine::Instance().m_assetsDB->GetMusic("townTheme"), 1000);

    return true;
}

bool GameScene::PreUpdate()
{
    return true;
}

bool GameScene::Update()
{

    canvas->UpdateCanvas();
    dialogueCanvas->UpdateCanvas();
    fade->UpdateCanvas();

    return true;
}

bool GameScene::PostUpdate()
{
    return true;
}

void GameScene::Render()
{
    canvas->RenderCanvas();
    dialogueCanvas->RenderCanvas();
    fade->RenderCanvas();
}


bool GameScene::CleanUp()
{
    delete dialogueCanvas;
    delete canvas;
    delete fade;
    Engine::Instance().m_render->RemoveFomRenderQueue(*this);
    return true;
}

