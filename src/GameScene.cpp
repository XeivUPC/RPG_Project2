#include "GameScene.h"
#include "Engine.h"
#include "ModuleRender.h"

#include "UITestingCG.h"

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

    canvas = new UITestingCG();
    Engine::Instance().m_render->AddToRenderQueue(*this);

    return true;
}

bool GameScene::PreUpdate()
{
    return true;
}

bool GameScene::Update()
{
    canvas->UpdateCanvas();
    return true;
}

bool GameScene::PostUpdate()
{
    return true;
}

void GameScene::Render()
{
    canvas->RenderCanvas();
}


bool GameScene::CleanUp()
{
    delete canvas;
    Engine::Instance().m_render->RemoveFomRenderQueue(*this);
    return true;
}

