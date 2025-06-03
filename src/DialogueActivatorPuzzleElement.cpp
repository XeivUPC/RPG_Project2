#include "DialogueActivatorPuzzleElement.h"
#include "PuzzleManager.h"


#include "Engine.h"
#include "ModuleRender.h"
#include "DrawingTools.h"
#include "ModuleAssetDatabase.h"
#include "GameScene.h"
#include "ModuleUpdater.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"
#include "DialogueSystem.h"

DialogueActivatorPuzzleElement::DialogueActivatorPuzzleElement()
{
}

DialogueActivatorPuzzleElement::~DialogueActivatorPuzzleElement()
{
}

bool DialogueActivatorPuzzleElement::Update()
{
    if (!isEnabled)
        return true;
    if (!isBeingTriggered && sensor.IsBeingTriggered()) {
        isBeingTriggered = true;
        Save();
        SendCall();
    }
    else if (isBeingTriggered && !sensor.IsBeingTriggered()) {
        isBeingTriggered = false;
        Save();
    }
    return true;
}

void DialogueActivatorPuzzleElement::Render()
{
}

void DialogueActivatorPuzzleElement::Initialize(string _id, Vector2Int _position, Vector2 _size, vector<string> _targets, bool _isEnabled, string _dialoguePath, int _dialogueState)
{
    id = _id;
    targets = _targets;
    dialoguePath = _dialoguePath;
    dialogueState = _dialogueState;
	isEnabled = _isEnabled;

    b2FixtureUserData sensorData;
    sensorData.pointer = (uintptr_t)(&sensor);


    ModulePhysics::Layer category, mask;

    body = Engine::Instance().m_physics->factory().CreateBox({ 0,0 }, _size.x, _size.y, sensorData);
    sensor.SetFixtureToTrack(body, 0);

    body->SetSensor(0, true);
    body->SetType(PhysBody::BodyType::Kinematic);
    category.flags.ground_layer = 1;
    mask.flags.player_layer = 1;
    body->SetFilter(0, category.rawValue, mask.rawValue, 0);


    PuzzleManager::Instance().AddPuzzleElement(_id, *this);
    if (!Load()) {

    }

    SetPosition(_position);
}

void DialogueActivatorPuzzleElement::RecieveCall(string _id, unordered_map<string, string> _params)
{
}

void DialogueActivatorPuzzleElement::SendCall()
{
    unordered_map<string, string> params;

    params["triggered"] = isBeingTriggered ? "true" : "false";

    for (const auto& target : targets)
    {
        params["target"] = target;
        PuzzleManager::Instance().onPuzzleCall.Trigger(id, params);
    }

    /// CallDialogue
    Engine::Instance().s_game->GetDialogue()->AddGameStateVariable("dialogueActivator-state",(float)dialogueState);
    Engine::Instance().s_game->SetDialogue(dialoguePath);
    Engine::Instance().s_game->SetState(GameScene::State::Dialogue);
}

void DialogueActivatorPuzzleElement::Complete()
{
}

void DialogueActivatorPuzzleElement::InitPoolObject()
{
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");
}

void DialogueActivatorPuzzleElement::ResetPoolObject()
{
    PuzzleManager::Instance().RemovePuzzleElement(*this);
    isBeingTriggered = false;
    SetPosition(Vector2Int(0, 0));
    targets.clear();

    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);

    delete body;
}

void DialogueActivatorPuzzleElement::SetPosition(Vector2 newPosition)
{
    position = newPosition;
    if (body != nullptr)
        body->SetPhysicPosition(position.x, position.y);
}

bool DialogueActivatorPuzzleElement::Load()
{
    bool done = true;
    if (PuzzleManager::Instance().HasPuzzleProperty(id, "isBeingTriggered")) {
        bool value = PuzzleManager::Instance().GetValueFromPuzzle(id, "isBeingTriggered") == "true";
        isBeingTriggered = value;
    }
    else
        done = false;

    if (done && PuzzleManager::Instance().HasPuzzleProperty(id, "isEnabled")) {
        bool value = PuzzleManager::Instance().GetValueFromPuzzle(id, "isEnabled") == "true";
        isEnabled = value;
    }
    else
        done = false;

    return done;
}

bool DialogueActivatorPuzzleElement::Save()
{
    PuzzleManager::Instance().SetValueFromPuzzle(id, "isBeingTriggered", isBeingTriggered ? "true" : "false");
    PuzzleManager::Instance().SetValueFromPuzzle(id, "isEnabled", isEnabled ? "true" : "false");
    return false;
}
