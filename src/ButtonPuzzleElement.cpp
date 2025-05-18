#include "ButtonPuzzleElement.h"
#include "PuzzleManager.h"

#include "Engine.h"
#include "ModuleRender.h"
#include "GameScene.h"
#include "ModuleUpdater.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"

ButtonPuzzleElement::ButtonPuzzleElement()
{

}

ButtonPuzzleElement::~ButtonPuzzleElement()
{

}

void ButtonPuzzleElement::Initialize(string _id, Vector2Int _position, bool value, vector<string> _targets)
{
	SetPosition(_position);
	id = _id;
	isPressed = value;
	targets = _targets;
}

void ButtonPuzzleElement::RecieveCall(string _id, unordered_map<string, string> _params)
{

}

void ButtonPuzzleElement::SendCall()
{
	unordered_map<string, string> params;

	params["buttonPressed"] = isPressed ? "true" : "false";

	for (const auto& target : targets)
	{
		params["target"] = target;
		PuzzleManager::Instance().onPuzzleCall.Trigger(id, params);
	}
}

void ButtonPuzzleElement::Complete()
{
}

void ButtonPuzzleElement::Interact()
{
	isPressed = !isPressed;
	SendCall();
}

void ButtonPuzzleElement::InitPoolObject()
{
	Engine::Instance().m_render->AddToRenderQueue(*this, *this);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");

	/// Collider
	ModulePhysics::Layer category, mask;

	body = Engine::Instance().m_physics->factory().CreateCircle({ 0,0 }, 1.0f);
	body->data = (uintptr_t)((IInteractuable*)this);
	body->SetSensor(0, true);
	category.flags.interactable_layer = 1;
	mask.flags.interactable_layer = 1;
	body->SetFilter(0, category.rawValue, mask.rawValue, 0);

}

void ButtonPuzzleElement::ResetPoolObject()
{
	isPressed = false;
	SetPosition(Vector2Int(0, 0));
	targets.clear();

	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_render->RemoveFromRenderQueue(*this);

	delete body;
}

void ButtonPuzzleElement::SetPosition(Vector2 newPosition)
{
	position = newPosition;
	if (body != nullptr)
		body->SetPhysicPosition(position.x, position.y);
}
