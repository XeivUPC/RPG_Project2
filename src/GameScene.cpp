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
#include "ItemList.h"
#include "Item.h"
#include "LOG.h"
#include "Party.h"
#include "Inventory.h"


#include "MissionHolder.h"
#include "MissionList.h"
#include "MissionManager.h"

#include "PuzzleManager.h"

#include "pugixml.hpp"


///Pooling
#include "Pooling.h"
#include "SimpleTilemapChanger.h"
#include "NpcCharacter.h"
#include "SimpleMapObject.h"
#include "OverworldItem.h"
#include "BirdFlock.h"
#include "Bird.h"
#include "ButtonPuzzleElement.h"
#include "BlockingPuzzleElement.h"
#include "TriggerPuzzleElement.h"
#include "DialogueActivatorPuzzleElement.h"
///

#include "FadeCG.h"
#include "UIDialogueBoxCG.h"
#include "CombatCG.h"
#include "PauseMenuCG.h"
#include "InventoryCG.h"
#include "GameplayCG.h"
#include "ScreenEffectsCG.h"

/// States
#include "GameState.h"
#include "DialogueGameState.h"
#include "PauseGameState.h"
#include "ExploringGameState.h"
#include "CombatGameState.h"
///



using namespace pugi;

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
    Pooling::Instance().CreatePool<SimpleTilemapChanger>(10);
    Pooling::Instance().CreatePool<SimpleMapObject>(30);
    Pooling::Instance().CreatePool<NpcCharacter>(10);
    Pooling::Instance().CreatePool<OverworldItem>(10);
    Pooling::Instance().CreatePool<BirdFlock>(10);
    Pooling::Instance().CreatePool<Bird>(20);
    Pooling::Instance().CreatePool<ButtonPuzzleElement>(3);
    Pooling::Instance().CreatePool<BlockingPuzzleElement>(3);
    Pooling::Instance().CreatePool<TriggerPuzzleElement>(3);
    Pooling::Instance().CreatePool<DialogueActivatorPuzzleElement>(3);

    fade = new FadeCG(33, 25, 17, 255);
    fade->FadeTo(1,0);
    fade->renderLayer = 9;



	dialogueSystem = new DialogueSystem();
    dialogueCanvas = new UIDialogueBoxCG(dialogueSystem);
    dialogueCanvas->renderLayer = 7;

    combatSystem = new CombatSystem();
    combatCanvas = new CombatCG(combatSystem);
    combatCanvas->renderLayer = 7;

    screenEffectsCanvas = new ScreenEffectsCG(5);
    pauseCanvas = new PauseMenuCG(7);

    gameplayCanvas = new GameplayCG(6);


   
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

    gameplayCanvas->SetUser(player);

    //MissionHolder* newMission = new MissionHolder(MissionList::Instance().MissionByID("mission;testing"));
    //MissionManager::Instance().AddMission(*newMission);

    //MissionHolder* newMission2 = new MissionHolder(MissionList::Instance().MissionByID("mission;testing2"));
    //MissionManager::Instance().AddMission(*newMission2);

    return true;
}

bool GameScene::PreUpdate()
{
    game_states[state]->PreUpdateState();

    return true;
}

bool GameScene::Update()
{   
    MissionManager::Instance().UpdateMissions();

    if(tilemaps.size()!=0)
        tilemaps[tilemaps.size()-1]->UpdateTilemap();

    UpdateRain();


    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
    {

        SetCombat(vector<string>{"character;zeryn", "character;rs_guard", "character;rs_guard"});
		SetState(State::Combat);
    }

    Engine::Instance().m_render->SortRenderQueueLayerByPosition(3);

    game_states[state]->UpdateState();

   

    return true;
}

bool GameScene::PostUpdate()
{
    fade->UpdateCanvas();
    game_states[state]->PostUpdateState();


    return true;
}


bool GameScene::CleanUp()
{

    delete dialogueCanvas;
    delete combatCanvas;
    delete pauseCanvas;
    delete gameplayCanvas;
    delete screenEffectsCanvas;
    //delete canvas;
    delete fade;
    delete dialogueSystem;
    delete combatSystem;

    for (; game_states.size() != 0;)
    {
        delete game_states.begin()->second;
        game_states.erase(game_states.begin()->first);
    }
    game_states.clear();

    for (; tilemaps.size() != 0;)
    {
        tilemaps[0]->CleanUp();
        delete tilemaps[0];
        tilemaps.erase(tilemaps.begin());
    }
    tilemaps.clear();

    player->CleanUp();
    delete player;
    cameraController->CleanUp();
    delete cameraController;

    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);

    DeletePoolMapObjects();

    MissionManager::Instance().Reset();
    MissionManager::Instance().ClearAllMissions();

    exitGame = false;

    return true;
}

void GameScene::SetDialogue(string path)
{
    dialogueSystem->LoadDialogueFromJSON(path);
    dialogueSystem->StartDialogue();
}

DialogueSystem* GameScene::GetDialogue()
{
    return dialogueSystem;
}

void GameScene::SetCombat(std::vector<string> enemyTeam)
{
    combatSystem->AddPartyToCombat(GetPlayer()->party->GetPartyIds(), CombatSystem::Ally);
    combatSystem->AddPartyToCombat(enemyTeam, CombatSystem::Enemy);
}

const CombatSystem* GameScene::GetCombat() const
{
    return combatSystem;
}


void GameScene::CheckTilesetInteriorState()
{
    if (tilemaps.size() == 0)
        return;
    Tilemap* tilemap = tilemaps[tilemaps.size() - 1];
    if (tilemap->GetTilemap().properties.count("IsInside")) {
        if (tilemap->GetTilemap().properties.at("IsInside").value == "true") {
            isInterior = true;
            PauseRain();
            screenEffectsCanvas->HideAmbient();
        }
        else {
            /// if was raining
            isInterior = false;
            ResumeRain();
            screenEffectsCanvas->ShowAmbient();
        }
    }
}

void GameScene::UpdateRain()
{
    //// Rain Logic

    if (isRaining) {
        rainTimer.Step(ModuleTime::deltaTime * timeScale);
        if (rainDuration <= rainTimer.ReadSec()) {
            rainTimer.Start();
            StopRain();
        }
    }
    else {
        rainTimer.Step(ModuleTime::deltaTime * timeScale);
        if (rainCooldown <= rainTimer.ReadSec()) {
            rainTimer.Start();

           
            std::uniform_int_distribution<int> chanceDist(1, 100);

            int chance = chanceDist(rng);
            if (chance <= 30)
                StartRain();
            else {
               
                std::uniform_real_distribution<float> cooldownDist(minRainCooldown, maxRainCooldown);
                rainCooldown = cooldownDist(rng);

            }
        }
    }

}

void GameScene::PauseRain()
{
    screenEffectsCanvas->StopRain();
}

void GameScene::ResumeRain()
{
    if (isRaining && !isInterior)
        screenEffectsCanvas->StartRain();
}

void GameScene::StopRain()
{
    std::uniform_real_distribution<float> cooldownDist(minRainCooldown, maxRainCooldown);
    rainCooldown = cooldownDist(rng);

    screenEffectsCanvas->StopRain();
    isRaining = false;
}

void GameScene::StartRain()
{
    std::uniform_real_distribution<float> durationDist(minRainDuration, maxRainDuration);
    rainDuration = durationDist(rng);

    isRaining = true;
    if(!isInterior)
        screenEffectsCanvas->StartRain();
}


void GameScene::SetState(State _newState)
{
    if (state == _newState)
        return;
    previous_state = state;
    state = _newState;
    if(previous_state != State::NONE___DO_NOT_USE)
        game_states[previous_state]->StateDeselected();
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

void GameScene::ChangeTilemap(string path, int entryPoint)
{
    fade->FadeTo(0.5f, 255);
    fade->onFadeEnd.Subscribe([this, path, entryPoint]() {SwapNewTilemap(path, entryPoint);  fade->FadeTo(0.5f, 0); });
    Engine::Instance().m_updater->PauseUpdateGroup("Entity");
}

void GameScene::AddTilemap(string path)
{
    fade->FadeTo(0.5f, 255);
    fade->onFadeEnd.Subscribe([this, path]() {CreateNewTilemap(path);    fade->FadeTo(0.5f, 0); });
    Engine::Instance().m_updater->PauseUpdateGroup("Entity");
}

void GameScene::RemoveLastTilemap()
{
    fade->FadeTo(0.5f, 255);
	fade->onFadeEnd.Subscribe([this]() {DeleteLastTilemap(); fade->FadeTo(0.5f, 0); });
    Engine::Instance().m_updater->PauseUpdateGroup("Entity");
}

void GameScene::CreateNewTilemap(string path)
{
    Engine::Instance().m_updater->ResumeUpdateGroup("Entity");

    if (path != "Assets/Map/Data/Rogue_Squadron_Headquarters.xml")
    {
        Engine::Instance().m_audio->PlaySFX(Engine::Instance().m_assetsDB->GetAudio("open_door"));
    }



    if (tilemaps.size() != 0) {
        tilemaps[tilemaps.size() - 1]->isVisible = false;
        ReturnToPoolMapObjects();
    }
    tilemaps.emplace_back(new Tilemap(path, 1));

    tilemaps[tilemaps.size() - 1]->CreateObjects();
    player->SetPosition(tilemaps[tilemaps.size() - 1]->GetSpawnPoint());
    cameraController->SetBounds(tilemaps[tilemaps.size() - 1]->GetPosition(), tilemaps[tilemaps.size() - 1]->GetTilemapSize());

    player->ClearFollowerPath();

    CheckTilesetInteriorState();

}

void GameScene::SwapNewTilemap(string path, int entryPoint)
{
    Engine::Instance().m_updater->ResumeUpdateGroup("Entity");

    Engine::Instance().m_audio->PlaySFX(Engine::Instance().m_assetsDB->GetAudio("change_area"));

   
    if (tilemaps.size() != 0) {
        ReturnToPoolMapObjects();
        //// Do Swap
        tilemaps[tilemaps.size() - 1]->CleanUp();
        delete tilemaps[tilemaps.size() - 1];
        tilemaps.pop_back();

        Tilemap* newTilemap = new Tilemap(path, 1);
        tilemaps.emplace_back(newTilemap);

        tilemaps[tilemaps.size() - 1]->CreateObjects();
        if (entryPoint != -1)
            player->SetPosition(tilemaps[tilemaps.size() - 1]->GetEntryPoint(entryPoint));
        else
            player->SetPosition(tilemaps[tilemaps.size() - 1]->GetSpawnPoint());

        cameraController->SetBounds(tilemaps[tilemaps.size() - 1]->GetPosition(), tilemaps[tilemaps.size() - 1]->GetTilemapSize());

        player->ClearFollowerPath();

        CheckTilesetInteriorState();
    }

}

void GameScene::DeleteLastTilemap()
{
    Engine::Instance().m_updater->ResumeUpdateGroup("Entity");

    Engine::Instance().m_audio->PlaySFX(Engine::Instance().m_assetsDB->GetAudio("close_door"));

    

    if (tilemaps.size() != 0) {

        ReturnToPoolMapObjects();

        tilemaps[tilemaps.size() - 1]->CleanUp();
        delete tilemaps[tilemaps.size() - 1];
        tilemaps.pop_back();

        if (tilemaps.size() != 0) {
            tilemaps[tilemaps.size() - 1]->isVisible = true;
            tilemaps[tilemaps.size() - 1]->CreateObjects();
            player->SetPosition(tilemaps[tilemaps.size() - 1]->GetSpawnPoint());
            cameraController->SetBounds(tilemaps[tilemaps.size() - 1]->GetPosition(), tilemaps[tilemaps.size() - 1]->GetTilemapSize());
        }

        player->ClearFollowerPath();

        CheckTilesetInteriorState();

    }
}




Tilemap* GameScene::GetLastTilemap()
{
    if (tilemaps.size() != 0)
        return  tilemaps[tilemaps.size() - 1];
    return nullptr;
}

int GameScene::GetTime()
{
    return (int)clock.ReadSec();
}

float GameScene::GetTimeScale()
{
    return timeScale;
}

PlayerCharacter* GameScene::GetPlayer() const
{
    return player;
}


void GameScene::FreshStart()
{
    PuzzleManager::Instance().ClearData();
    CreateNewTilemap("Assets/Map/Data/Rogue_Squadron_Headquarters.xml");
    MissionManager::Instance().AddMission(*new MissionHolder(MissionList::Instance().MissionByID("mission;tutorial")));
    clock = StepTimer(3600*12);
    screenEffectsCanvas->RecalculateAmbientFadeColors();
}


void GameScene::AskForLoadSaveData()
{
    fade->FadeTo(0.5f, 255);

    fade->onFadeEnd.Subscribe([this]() {LoadGameSaveData();  fade->FadeTo(0.5f, 0);  SetState(State::Exploring); });
}

void GameScene::ReturnToPoolMapObjects()
{
    Pooling::Instance().ReturnAllToPool<SimpleTilemapChanger>();
    Pooling::Instance().ReturnAllToPool<SimpleMapObject>();
    Pooling::Instance().ReturnAllToPool<NpcCharacter>();
    Pooling::Instance().ReturnAllToPool<OverworldItem>();
    Pooling::Instance().ReturnAllToPool<BirdFlock>();
    Pooling::Instance().ReturnAllToPool<Bird>();
    Pooling::Instance().ReturnAllToPool<ButtonPuzzleElement>();
    Pooling::Instance().ReturnAllToPool<BlockingPuzzleElement>();
    Pooling::Instance().ReturnAllToPool<TriggerPuzzleElement>();
    Pooling::Instance().ReturnAllToPool<DialogueActivatorPuzzleElement>();
}

void GameScene::DeletePoolMapObjects()
{
    Pooling::Instance().DeletePool<SimpleTilemapChanger>(true);
    Pooling::Instance().DeletePool<SimpleMapObject>(true);
    Pooling::Instance().DeletePool<NpcCharacter>(true);
    Pooling::Instance().DeletePool<OverworldItem>(true);
    Pooling::Instance().DeletePool<BirdFlock>(true);
    Pooling::Instance().DeletePool<Bird>(true);
    Pooling::Instance().DeletePool<ButtonPuzzleElement>(true);
    Pooling::Instance().DeletePool<BlockingPuzzleElement>(true);
    Pooling::Instance().DeletePool<TriggerPuzzleElement>(true);
    Pooling::Instance().DeletePool<DialogueActivatorPuzzleElement>(true);
}


void GameScene::LoadGameSaveData()
{
    LOG("Loading Game");

    PuzzleManager::Instance().LoadAllData();

    xml_document file;
    pugi::xml_parse_result result = file.load_file(savePath.c_str());
    if (result != NULL)
    {

        player->party->ClearParty();
		player->party->ClearMemebers();
		player->inventory->ClearAllItems();
		MissionManager::Instance().ClearAllMissions();

        for (size_t i = 0; i < tilemaps.size(); i++)
        {
            tilemaps[i]->CleanUp();
            delete tilemaps[i];
        }
        tilemaps.clear();

        ReturnToPoolMapObjects();


        xml_node rootNode = file.child("game");

        xml_node playerNode = rootNode.child("player");
        xml_node mapNode = rootNode.child("map");

        Vector2Int playerPosData = { 0,0 };
        playerPosData.x = playerNode.child("position").attribute("x").as_int();
        playerPosData.y = playerNode.child("position").attribute("y").as_int();
        

        xml_node partyNode = playerNode.child("party");
        xml_node activeNode = partyNode.child("active");
        xml_node inactiveNode = partyNode.child("inactive");

        for (xml_node member = inactiveNode.child("member"); member; member = member.next_sibling("member"))
        {
            string id = member.attribute("id").as_string();
            player->party->AddMemeber(id);
            xml_node inventoryNode = member.child("inventory");
            if (inventoryNode) {
                Party::Member* character = player->party->GetFullCharacterDataFromMembers(id);
                for (xml_node slot = inventoryNode.child("item"); slot; slot = slot.next_sibling("item"))
                {
                    if (!slot.empty()) {
                        string itemId = slot.attribute("id").as_string();
                        int amount = slot.attribute("amount").as_int();
						Item* item = ItemList::Instance().ItemByID(itemId);
                        character->inventory->AddItem(InventoryItem(item), amount);
                    }
                }
            }
        }

        for (xml_node member = activeNode.child("member"); member; member = member.next_sibling("member"))
        {
            string id = member.attribute("id").as_string();
            player->party->AddPartyMemeber(id);
			
        }

        /// Load Inventory
        vector<InventorySlot>& slots = player->inventory->GetSlotsDataModifiable();
        xml_node inventoryNode = playerNode.child("inventory");
        int inventoryIndex = 0;
        for (xml_node itemInventoryNode = inventoryNode.child("item"); itemInventoryNode; itemInventoryNode = itemInventoryNode.next_sibling("item"))
        {
            if (itemInventoryNode.attribute("id")) {
			    slots[inventoryIndex].item = new InventoryItem(ItemList::Instance().ItemByID(itemInventoryNode.attribute("id").as_string()));
			    slots[inventoryIndex].count = itemInventoryNode.attribute("amount").as_int();

            }
            inventoryIndex++;
            
        }
        player->inventory->onInventoryChanged.Trigger();

        /// Load Missions
        xml_node missionsNode = playerNode.child("missions");
        for (xml_node missionNode = missionsNode.child("mission"); missionNode; missionNode = missionNode.next_sibling("mission"))
        {

            string missionId = missionNode.attribute("id").as_string();
            int status = missionNode.attribute("status").as_int();

            MissionHolder* newMission = new MissionHolder(MissionList::Instance().MissionByID(missionId));

            newMission->SetState((MissionHolder::State)status);
	
            MissionManager::Instance().AddMission(*newMission);
        }
        MissionManager::Instance().UpdateMissionsStatus();
		

		/// Load Map
        isRaining = mapNode.child("raining").attribute("value").as_bool();
        clock = StepTimer(mapNode.child("time").attribute("value").as_float());

        xml_node tilemapsNode = mapNode.child("tilemaps");
        for (xml_node tilemap = tilemapsNode.child("tilemap"); tilemap; tilemap = tilemap.next_sibling("tilemap"))
        {

            Vector2Int tilemapSpawnPoint = { 0, 0 };
            tilemapSpawnPoint.x = tilemap.attribute("spawnpoint-x").as_int();
            tilemapSpawnPoint.y = tilemap.attribute("spawnpoint-y").as_int();

            bool isLast = tilemap.next_sibling("tilemap") == nullptr;
            if (isLast)
                CreateNewTilemap(tilemap.attribute("path").as_string());
            else {
                tilemaps.emplace_back(new Tilemap(tilemap.attribute("path").as_string(), 1));
                tilemaps[tilemaps.size() - 1]->isVisible = false;
            }

			tilemaps[tilemaps.size() - 1]->SetSpawnPoint(tilemapSpawnPoint);
        }     


        Pooling::Instance().ReturnAllToPool<OverworldItem>();
        xml_node itemsNode = mapNode.child("items");
        for (xml_node item = itemsNode.child("item"); item; item = item.next_sibling("item"))
        {
            Vector2Int itemPosition = { 0, 0 };
            itemPosition.x = item.attribute("position-x").as_int();
            itemPosition.y = item.attribute("position-y").as_int();

            string itemId = item.attribute("id").as_string();
            int itemAmount = item.attribute("amount").as_int();


            auto itemObject = Pooling::Instance().AcquireObject<OverworldItem>();
            itemObject->Initialize(ItemList::Instance().ItemByID(itemId), itemAmount, itemPosition);
        }


        pauseCanvas->inventory->LoadUseSlot();

        player->SetPosition(playerPosData);




        if (!isRaining)
            StopRain();
        else
            StartRain();



        screenEffectsCanvas->RecalculateAmbientFadeColors();
    }
    else {
        LOG("Game couldn't be loaded");
    }

  
}


void GameScene::SaveGameSaveData()
{

    LOG("Saving Game");

	PuzzleManager::Instance().SaveAllData();

    xml_document file;
    pugi::xml_parse_result result = file.load_file(savePath.c_str());
    if (result != NULL)
    {
        xml_node rootNode = file.child("game");

        xml_node playerNode = rootNode.child("player");
        xml_node mapNode = rootNode.child("map");

        /// Save Position
		playerNode.child("position").attribute("x").set_value(player->GetPosition().x);
		playerNode.child("position").attribute("y").set_value(player->GetPosition().y);

        //// SaveParty
        vector<string> partyMembers = player->party->GetPartyIds();
        vector<Party::Member*> unlockedMembers = player->party->GetFullMemebersData();
        xml_node activeNode = playerNode.child("party").child("active");
        xml_node inactiveNode = playerNode.child("party").child("inactive");
        activeNode.remove_children();
        inactiveNode.remove_children();

        for (string id : partyMembers) {
            activeNode.append_child("member").append_attribute("id").set_value(id.c_str());
        }

        for (Party::Member* character : unlockedMembers) {
            xml_node memberNode = inactiveNode.append_child("member");
            memberNode.append_attribute("id").set_value(character->id.c_str());
            memberNode.remove_child("inventory");
            if (character->inventory->GetUsedSlots() > 0) {
                xml_node inventoryNode = memberNode.append_child("inventory");
                for (auto slot : character->inventory->GetSlotsData()) {
					xml_node itemNode = inventoryNode.append_child("item");
					if (!slot.IsEmpty()) {
						itemNode.append_attribute("id").set_value(slot.item->GetReference()->id.c_str());
						itemNode.append_attribute("amount").set_value(slot.count);
					}
                }
            }

        }

        /// Save Inventory
        const vector<InventorySlot>& slots = player->inventory->GetSlotsData();
        xml_node inventoryNode = playerNode.child("inventory");
        inventoryNode.remove_children();
        for (InventorySlot itemSlot : slots) {
            xml_node itemInventoryNode = inventoryNode.append_child("item");
            if (itemSlot.item != nullptr) {
                itemInventoryNode.append_attribute("id").set_value(itemSlot.item->GetReference()->id.c_str());
                itemInventoryNode.append_attribute("amount").set_value(itemSlot.count);
            }
            else {

            }
        }

        /// Save Missions
        const vector<MissionHolder*>& missions = MissionManager::Instance().GetMissions();
        xml_node missionsNode = playerNode.child("missions");
        missionsNode.remove_children();
        for (MissionHolder* mission : missions) {
            xml_node missionNode = missionsNode.append_child("mission");
            missionNode.append_attribute("id").set_value(mission->GetId().c_str());
            missionNode.append_attribute("status").set_value((int)mission->GetState());
        }


		mapNode.child("raining").attribute("value").set_value(isRaining);
		mapNode.child("time").attribute("value").set_value(clock.ReadSec());

        xml_node tilemapsNode = mapNode.child("tilemaps");
        tilemapsNode.remove_children();

        for (Tilemap* tilemap : tilemaps) {
            xml_node tilemapNode  = tilemapsNode.append_child("tilemap");
			tilemapNode.append_attribute("path").set_value(tilemap->GetTilemap().path.c_str());

            Vector2 spawnPointData = tilemap->GetSpawnPoint();
			tilemapNode.append_attribute("spawnpoint-x").set_value(spawnPointData.x);
			tilemapNode.append_attribute("spawnpoint-y").set_value(spawnPointData.y);
        }

        xml_node itemsNode = mapNode.child("items");
        itemsNode.remove_children();

        vector<shared_ptr<OverworldItem>> itemsActive = Pooling::Instance().AcquireActiveObjects<OverworldItem>();
        for (const auto& item : itemsActive)
        {
            xml_node itemNode = itemsNode.append_child("item");

			Vector2 itemPos = item->GetPosition();
			string itemId = item->GetId();
			int itemAmount = item->GetAmount();

            itemNode.append_attribute("id").set_value(itemId.c_str());
            itemNode.append_attribute("amount").set_value(itemAmount);
            itemNode.append_attribute("position-x").set_value(itemPos.x);
            itemNode.append_attribute("position-y").set_value(itemPos.y);
        }

        file.save_file(savePath.c_str());

		CharacterDatabase::Instance().SaveDatabase();

        pauseCanvas->inventory->SaveUseSlot();

    }
    else {
        LOG("Game couldn't be saved");
    }
}



