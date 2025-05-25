#include "ButtonPuzzleElement.h"
#include "PuzzleManager.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleRender.h"
#include "DrawingTools.h"
#include "GameScene.h"
#include "ModuleUpdater.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"

ButtonPuzzleElement::ButtonPuzzleElement()
{
	texture = Engine::Instance().m_assetsDB->GetTexture("puzzle");
	renderLayer = 3;
	renderOffsetSorting = { 0,8 };
}

ButtonPuzzleElement::~ButtonPuzzleElement()
{

}

void ButtonPuzzleElement::Render()
{
	SDL_Rect rect = { 0,0,16,16 };
	if (isPressed)
		rect = { 16,0,16,16 };
	Engine::Instance().m_render->painter().RenderTexture(*texture, position - Vector2{0,3}, & rect, { 1,1 }, 0, { 0.5f,0.5f });
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

	params["triggered"] = isPressed ? "true" : "false";

	for (const auto& target : targets)
	{
		params["target"] = target;
		PuzzleManager::Instance().onPuzzleCall.Trigger(id, params);
	}
}

void ButtonPuzzleElement::Complete()
{
}

void ButtonPuzzleElement::Interact(Vector2 from)
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
	body->SetType(PhysBody::BodyType::Kinematic);
	category.flags.interactable_layer = 1;
	mask.flags.interactable_layer = 1;
	body->SetFilter(0, category.rawValue, mask.rawValue, 0);

	category.rawValue = 0;
	mask.rawValue = 0;

	int index = Engine::Instance().m_physics->factory().AddBox(body, { 0, PIXEL_TO_METERS(3) }, PIXEL_TO_METERS(16),PIXEL_TO_METERS(4));
	category.flags.ground_layer = 1;
	mask.flags.player_layer = 1;
	body->SetFilter(index, category.rawValue, mask.rawValue, 0);
	body->SetSensor(index, false);

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
