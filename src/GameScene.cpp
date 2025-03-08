#include "GameScene.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleAssetDatabase.h"
#include "ModuleAudio.h"
#include "Tilemap.h"

#include "FadeCG.h"
#include "UITestingCG.h"
#include "UIDialogueBoxCG.h"
#include "UIDialogueBoxCG.h"

/// States
#include "GameState.h"
#include "DialogueGameState.h"
///

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


    //// Create States

    game_states[State::Dialogue] = new DialogueGameState();

    ////

    tilemaps.emplace_back(new Tilemap("Assets/Map/Data/Testing_Zone.xml"));
    tilemaps[0]->scale = 2;

    return true;
}

bool GameScene::PreUpdate()
{
    game_states[state]->PreUpdateState();
    return true;
}

bool GameScene::Update()
{   
    for (size_t i = 0; i < tilemaps.size(); i++)
    {
        tilemaps[i]->UpdateTilemap();
    }

    canvas->UpdateCanvas();

    game_states[state]->UpdateState();

    fade->UpdateCanvas();

    return true;
}

bool GameScene::PostUpdate()
{
    game_states[state]->PostUpdateState();
    return true;
}

void GameScene::Render()
{
    for (size_t i = 0; i < tilemaps.size(); i++)
    {
        tilemaps[i]->RenderTilemap();
    }

    canvas->RenderCanvas();

    game_states[state]->RenderState();

    fade->RenderCanvas();  
}


bool GameScene::CleanUp()
{
    delete dialogueCanvas;
    delete canvas;
    delete fade;

    for (; game_states.size() != 0;)
    {
        delete game_states.begin()->second;
        game_states.erase(game_states.begin()->first);
    }
    game_states.clear();

    for (size_t i = 0; i < tilemaps.size(); i++)
    {
        delete tilemaps[i];
    }
    tilemaps.clear();

    Engine::Instance().m_render->RemoveFomRenderQueue(*this);
    return true;
}

void GameScene::SetState(State _newState)
{
    game_states[state]->StateDeselected();
    state = _newState;
    game_states[state]->StateSelected();
}

GameScene::State GameScene::GetState()
{
    return state;
}

