#include "GameScene.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleUpdater.h"
#include "ModuleAssetDatabase.h"
#include "ModuleAudio.h"
#include "ModuleTime.h"
#include "Tilemap.h"
#include "PlayerCharacter.h"
#include "CameraController.h"
#include "DialogueSystem.h"
#include "CombatSystem.h"

///Pooling
#include "Pooling.h"
#include "Building.h"
#include "NpcCharacter.h"
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
#include "CombatGameState.h"
///

GameScene::GameScene(bool start_active) : ModuleScene(start_active)
{
}

GameScene::~GameScene()
{
    
}

void GameScene::SetDialogue(string path)
{
    dialogueSystem->LoadDialogueFromJSON(path);
    dialogueSystem->StartDialogue();
}

bool GameScene::Init()
{
    return true;
}

bool GameScene::Start()
{
    Pooling::Instance().CreatePool<Building>(10);
    Pooling::Instance().CreatePool<SimpleMapObject>(30);
    Pooling::Instance().CreatePool<NpcCharacter>(10);

    fade = new FadeCG(33, 25, 17, 255);
    fade->FadeTo(1,0);
    fade->renderLayer = 9;

    //canvas = new UITestingCG();
    //canvas->renderLayer = 6;

	dialogueSystem = new DialogueSystem();
    dialogueCanvas = new UIDialogueBoxCG(dialogueSystem);
    dialogueCanvas->renderLayer = 7;

    combatSystem = new CombatSystem();
    dialogueCanvas = new UIDialogueBoxCG(dialogueSystem);
    dialogueCanvas->renderLayer = 7;

    pauseCanvas = new PauseMenuCG();
    pauseCanvas->renderLayer = 7;

    Engine::Instance().m_audio->PlayMusicAsync(Engine::Instance().m_assetsDB->GetMusic("townTheme"), 1000);
    Engine::Instance().m_render->SetCameraZoom(1.5f);
    Engine::Instance().m_render->SetCameraPosition(Vector2{0, 0});
    
    //// Create States

    game_states[State::Exploring] = new ExploringGameState();
    game_states[State::Exploring]->StateDeselected();
    game_states[State::Dialogue] = new DialogueGameState();
    game_states[State::Dialogue]->StateDeselected();
    game_states[State::Menu] = new PauseGameState();
    game_states[State::Menu]->StateDeselected();
    game_states[State::Combat] = new CombatGameState();
    game_states[State::Combat]->StateDeselected();

    SetState(State::Exploring);

    ////

    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);
   
    player = new PlayerCharacter();
    cameraController = new CameraController();
    cameraController->SetTarget(player);
    cameraController->SetOffset({ -LOGIC_SCREEN_WIDTH / 2, -LOGIC_SCREEN_HEIGHT / 2 });
    

    AddTilemap("Assets/Map/Data/Rogue_Squadron_Headquarters.xml");


    return true;
}

bool GameScene::PreUpdate()
{
    game_states[state]->PreUpdateState();

    return true;
}

bool GameScene::Update()
{   
    if(tilemaps.size()!=0)
        tilemaps[tilemaps.size()-1]->UpdateTilemap();

    for (size_t i = 0; i < entities.size(); i++)
    {
        entities[i]->Update();
    }

    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
    {
        SetCombat()
    }

    Engine::Instance().m_render->SortRenderQueueLayerByPosition(3);

    //canvas->UpdateCanvas();

    game_states[state]->UpdateState();

    fade->UpdateCanvas();

    return true;
}

bool GameScene::PostUpdate()
{
    game_states[state]->PostUpdateState();

    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
        RemoveLastTilemap();
    return true;
}


bool GameScene::CleanUp()
{
    delete dialogueCanvas;
    delete pauseCanvas;
    //delete canvas;
    delete fade;
    delete dialogueSystem;
   

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

    player->CleanUp();
    delete player;
    cameraController->CleanUp();
    delete cameraController;

    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);

    Pooling::Instance().DeletePool<Building>(true);
    Pooling::Instance().DeletePool<SimpleMapObject>(true);
    Pooling::Instance().DeletePool<NpcCharacter>(true);

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

GameState* GameScene::GetGameState()
{
    return game_states[state];
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

void GameScene::AddTilemap(string path)
{
    if (tilemaps.size() != 0) {
        tilemaps[tilemaps.size() - 1]->isVisible = false;
        Pooling::Instance().ReturnAllToPool<Building>();
        Pooling::Instance().ReturnAllToPool<SimpleMapObject>();
        Pooling::Instance().ReturnAllToPool<NpcCharacter>();
    }
    tilemaps.emplace_back(new Tilemap(path, 1));

    tilemaps[tilemaps.size() - 1]->CreateObjects();
    player->SetPosition(tilemaps[tilemaps.size() - 1]->GetSpawnPoint());
    cameraController->SetBounds(tilemaps[tilemaps.size() - 1]->GetPosition(), tilemaps[tilemaps.size() - 1]->GetTilemapSize());
}

Tilemap* GameScene::GetLastTilemap()
{
    if (tilemaps.size() != 0)
        return  tilemaps[tilemaps.size() - 1];
    return nullptr;
}

void GameScene::RemoveLastTilemap()
{
    if (tilemaps.size() != 0) {

        Pooling::Instance().ReturnAllToPool<Building>();
        Pooling::Instance().ReturnAllToPool<SimpleMapObject>();
        Pooling::Instance().ReturnAllToPool<NpcCharacter>();

        delete tilemaps[tilemaps.size() - 1];
        tilemaps.pop_back();

        if (tilemaps.size() != 0) {
            tilemaps[tilemaps.size() - 1]->isVisible = true;
            tilemaps[tilemaps.size() - 1]->CreateObjects();
            player->SetPosition(tilemaps[tilemaps.size() - 1]->GetSpawnPoint());
            cameraController->SetBounds(tilemaps[tilemaps.size() - 1]->GetPosition(), tilemaps[tilemaps.size() - 1]->GetTilemapSize());
        }
    }


    
}

