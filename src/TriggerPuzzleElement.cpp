#include "TriggerPuzzleElement.h"
#include "PuzzleManager.h"


#include "Engine.h"
#include "ModuleRender.h"
#include "DrawingTools.h"
#include "ModuleAssetDatabase.h"
#include "GameScene.h"
#include "ModuleUpdater.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"

TriggerPuzzleElement::TriggerPuzzleElement()
{
	texture = Engine::Instance().m_assetsDB->GetTexture("puzzle");
	renderLayer = 2;

}

TriggerPuzzleElement::~TriggerPuzzleElement()
{
}

bool TriggerPuzzleElement::Update()
{
	if (!isBeingTriggered && sensor.IsBeingTriggered()) {
		if (onEnter) {
			isBeingTriggered = true;
			Save();
			SendCall();
		}
	}
	else if(isBeingTriggered && !sensor.IsBeingTriggered()){
		if (onExit) {
			isBeingTriggered = false;
			Save();
			SendCall();
		}
	}
	return true;
}

void TriggerPuzzleElement::Render()
{
	SDL_Rect rect = {32,0,16,16};
	if (isBeingTriggered)
		rect = { 48,0,16,16 };
	Engine::Instance().m_render->painter().RenderTexture(*texture, position, &rect, { 1,1 }, 0, {0.5f,0.5f});
}

void TriggerPuzzleElement::Initialize(string _id, Vector2Int _position, float _size, vector<string> _targets, bool callOnEnter, bool callOnExit)
{
	id = _id;
	targets = _targets;
	onEnter = callOnEnter;
	onExit = callOnExit;

	b2FixtureUserData sensorData;
	sensorData.pointer = (uintptr_t)(&sensor);


	ModulePhysics::Layer category, mask;

	body = Engine::Instance().m_physics->factory().CreateCircle({ 0,0 }, _size, sensorData);
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

void TriggerPuzzleElement::RecieveCall(string _id, unordered_map<string, string> _params)
{
}

void TriggerPuzzleElement::SendCall()
{
	unordered_map<string, string> params;

	params["triggered"] = isBeingTriggered ? "true" : "false";

	for (const auto& target : targets)
	{
		params["target"] = target;
		PuzzleManager::Instance().onPuzzleCall.Trigger(id, params);
	}
}

void TriggerPuzzleElement::Complete()
{
}

void TriggerPuzzleElement::InitPoolObject()
{
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");
	Engine::Instance().m_render->AddToRenderQueue(*this, *this);

}

void TriggerPuzzleElement::ResetPoolObject()
{
	PuzzleManager::Instance().RemovePuzzleElement(*this);
	isBeingTriggered = false;
	SetPosition(Vector2Int(0, 0));
	targets.clear();

	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_render->RemoveFromRenderQueue(*this);

	delete body;
}

void TriggerPuzzleElement::SetPosition(Vector2 newPosition)
{
	position = newPosition;
	if (body != nullptr)
		body->SetPhysicPosition(position.x, position.y);
}

bool TriggerPuzzleElement::Load()
{
	if (PuzzleManager::Instance().HasPuzzleProperty(id, "isBeingTriggered")) {
		bool value = PuzzleManager::Instance().GetValueFromPuzzle(id, "isBeingTriggered") == "true";
		isBeingTriggered = value;

		return true;
	}
	return false;
}

bool TriggerPuzzleElement::Save()
{
	PuzzleManager::Instance().SetValueFromPuzzle(id, "isBeingTriggered", isBeingTriggered ? "true" : "false");
	return true;
}
