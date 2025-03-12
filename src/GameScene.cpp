#include "GameScene.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleUpdater.h"
#include "ModuleAssetDatabase.h"
#include "ModuleAudio.h"
#include "ModuleTime.h"
#include "Tilemap.h"

///Pooling
#include "Pooling.h"
#include "Building.h"
#include "SimpleMapObject.h"
///

#include "FadeCG.h"
#include "UIDialogueBoxCG.h"
#include "PauseMenuCG.h"

/// States
#include "GameState.h"
#include "DialogueGameState.h"
#include "PauseGameState.h"
#include "ExploringGameState.h"
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
    Pooling::Instance().CreatePool<Building>(10);
    Pooling::Instance().CreatePool<SimpleMapObject>(30);

    fade = new FadeCG(33, 25, 17, 255);
    fade->FadeTo(1,0);
    fade->renderLayer = 9;

    //canvas = new UITestingCG();
    //canvas->renderLayer = 6;

    dialogueCanvas = new UIDialogueBoxCG();
    dialogueCanvas->renderLayer = 7;

    pauseCanvas = new PauseMenuCG();
    pauseCanvas->renderLayer = 7;

    Engine::Instance().m_audio->PlayMusicAsync(Engine::Instance().m_assetsDB->GetMusic("townTheme"), 1000);
    Engine::Instance().m_render->SetCameraZoom(1.5f);
    Engine::Instance().m_render->SetCameraPosition({0, 0});
    
    //// Create States

    game_states[State::Exploring] = new ExploringGameState();
    game_states[State::Exploring]->StateDeselected();
    game_states[State::Dialogue] = new DialogueGameState();
    game_states[State::Dialogue]->StateDeselected();
    game_states[State::Menu] = new PauseGameState();
    game_states[State::Menu]->StateDeselected();

    SetState(State::Dialogue);

    ////

    tilemaps.emplace_back(new Tilemap("Assets/Map/Data/Rogue_Squadron_Headquarters.xml",1));

    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);

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

    for (size_t i = 0; i < entities.size(); i++)
    {
        entities[i]->Update();
    }

    Vector2 dir = { 0,0 };
    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
        dir.y -= 40;
    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
        dir.y += 40;
    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
        dir.x -= 40;
    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
        dir.x += 40;

    Engine::Instance().m_render->MoveCamera(dir*(float)ModuleTime::deltaTime);

    //canvas->UpdateCanvas();

    game_states[state]->UpdateState();

    fade->UpdateCanvas();

    return true;
}

bool GameScene::PostUpdate()
{
    game_states[state]->PostUpdateState();
    return true;
}


bool GameScene::CleanUp()
{
    delete dialogueCanvas;
    delete pauseCanvas;
    //delete canvas;
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

    for (size_t i = 0; i < entities.size(); i++)
    {
        entities[i]->CleanUp();
        delete entities[i];
    }
    entities.clear();

    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);

    Pooling::Instance().DeletePool<Building>(true);
    Pooling::Instance().DeletePool<SimpleMapObject>(true);

    exitGame = false;

    return true;
}

void GameScene::SetState(State _newState)
{
    if (state == _newState)
        return;
    previous_state = state;
    if(state != State::NONE___DO_NOT_USE)
        game_states[state]->StateDeselected();
    state = _newState;
    game_states[state]->StateSelected();
}

GameScene::State GameScene::GetState()
{
    return state;
}

void GameScene::SetPreviousState()
{
    if (state == previous_state || state == State::NONE___DO_NOT_USE)
        return;
    SetState(previous_state);
}

void GameScene::ExitGame()
{
    exitGame = true;
    fade->FadeTo(0.5f, 255);
}

