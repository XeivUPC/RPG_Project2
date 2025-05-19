#include "BlockingPuzzleElement.h"
#include "PuzzleManager.h"


#include "Engine.h"
#include "ModuleRender.h"
#include "GameScene.h"
#include "ModuleUpdater.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"

BlockingPuzzleElement::BlockingPuzzleElement()
{
}

BlockingPuzzleElement::~BlockingPuzzleElement()
{
}

void BlockingPuzzleElement::Initialize(string _id, Vector2Int _position, Vector2 _size, bool blocks)
{
	ModulePhysics::Layer category, mask;

	body = Engine::Instance().m_physics->factory().CreateBox({ 0,0 }, _size.x, _size.y);
	body->SetSensor(0, true);
	body->SetType(PhysBody::BodyType::Kinematic);
	category.flags.ground_layer = 1;
	mask.flags.player_layer = 1;
	body->SetFilter(0, category.rawValue, mask.rawValue, 0);



	SetPosition(_position);
	id = _id;
	isBlocking = blocks;
	if (isBlocking)
		Enable();
	else
		Disable();

}

void BlockingPuzzleElement::RecieveCall(string _id, unordered_map<string, string> _params)
{
	if (_params.count("target") != 0) {
		if (_params["target"] != id)
			return;
		else
		{
			if (_params.count("buttonPressed") != 0) {

				isBlocking = !isBlocking;

				if (isBlocking)
					Enable();
				else
					Disable();
			}
		}
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
	eventId = PuzzleManager::Instance().onPuzzleCall.Subscribe([this](string _id, unordered_map<string,string> _params) {RecieveCall(_id, _params);});


	
}

void BlockingPuzzleElement::ResetPoolObject()
{
	PuzzleManager::Instance().onPuzzleCall.Unsubscribe(eventId);
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
