#include "BlockingPuzzleElement.h"
#include "PuzzleManager.h"


#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleRender.h"
#include "DrawingTools.h"
#include "GameScene.h"
#include "ModuleUpdater.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"

BlockingPuzzleElement::BlockingPuzzleElement()
{
	renderLayer = 3;
	renderOffsetSorting = { 0,0 };
}

BlockingPuzzleElement::~BlockingPuzzleElement()
{
}

void BlockingPuzzleElement::Render()
{
	if(isBlocking)
		Engine::Instance().m_render->painter().RenderBox(position, size, { 1,1 }, { 0.5f,0.5f }, true);
}

void BlockingPuzzleElement::Initialize(string _id, Vector2Int _position, Vector2 _size, bool blocks)
{
	id = _id;
	size = { (float)METERS_TO_PIXELS(_size.x),(float)METERS_TO_PIXELS(_size.y) };
	renderOffsetSorting = {0,(int)(size.y/2)};

	ModulePhysics::Layer category, mask;

	body = Engine::Instance().m_physics->factory().CreateBox({ 0,0 }, _size.x, _size.y);
	body->SetSensor(0, true);
	body->SetType(PhysBody::BodyType::Kinematic);
	category.flags.ground_layer = 1;
	mask.flags.player_layer = 1;
	body->SetFilter(0, category.rawValue, mask.rawValue, 0);


	PuzzleManager::Instance().AddPuzzleElement(_id, *this);
	SetPosition(_position);

	if (!Load()) {
		isBlocking = blocks;
		if (isBlocking)
			Enable();
		else
			Disable();
	}

}

void BlockingPuzzleElement::RecieveCall(string _id, unordered_map<string, string> _params)
{
	if (_params.count("target") != 0) {
		if (_params["target"] != id)
			return;
		else
		{
			if (_params.count("triggered") != 0) {

				isBlocking = !isBlocking;
				if (isBlocking)
					Enable();
				else
					Disable();
				Save();
			}
		}
	}
	else {
		
	}
}

void BlockingPuzzleElement::SendCall()
{
	
}

void BlockingPuzzleElement::Complete()
{
}

void BlockingPuzzleElement::InitPoolObject()
{
	Engine::Instance().m_render->AddToRenderQueue(*this, *this);
	eventId = PuzzleManager::Instance().onPuzzleCall.Subscribe([this](string _id, unordered_map<string,string> _params) {RecieveCall(_id, _params);});
}

void BlockingPuzzleElement::ResetPoolObject()
{
	PuzzleManager::Instance().RemovePuzzleElement(*this);
	if (eventId.id != -1) {
		PuzzleManager::Instance().onPuzzleCall.Unsubscribe(eventId);
		eventId.id = -1;
	}
	Engine::Instance().m_render->RemoveFromRenderQueue(*this);
	delete body;
}

void BlockingPuzzleElement::SetPosition(Vector2 newPosition)
{
	position = newPosition;
	if (body != nullptr)
		body->SetPhysicPosition(position.x, position.y);
}

void BlockingPuzzleElement::Enable()
{
	isBlocking = true;
	body->SetSensor(0, false);
}

void BlockingPuzzleElement::Disable()
{
	isBlocking = false;
	body->SetSensor(0, true);
}

bool BlockingPuzzleElement::Load()
{
	if (PuzzleManager::Instance().HasPuzzleProperty(id, "isBlocking")) {
		bool value = PuzzleManager::Instance().GetValueFromPuzzle(id, "isBlocking") == "true";
		if (value)
			Enable();
		else
			Disable();
		isBlocking = value;

		return true;
	}
	return false;
}

bool BlockingPuzzleElement::Save()
{
	PuzzleManager::Instance().SetValueFromPuzzle(id, "isBlocking", isBlocking ? "true" : "false");
	return true;
}
