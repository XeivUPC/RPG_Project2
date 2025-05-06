#include "OverworldItem.h"
#include "ModuleAssetDatabase.h"
#include "Engine.h"
#include "ModuleUpdater.h"
#include "ModuleRender.h"
#include "Pooling.h"
#include "Item.h"
#include "DrawingTools.h"

#include "ModulePhysics.h"
#include "PhysicFactory.h"

void OverworldItem::Reset()
{
}

OverworldItem::OverworldItem()
{
	renderLayer = 3;
}

OverworldItem::~OverworldItem()
{
	texture = nullptr;
}

bool OverworldItem::Update()
{
	if (interactionSensor.OnTriggerEnter()) {
		PickUp();
	}
	return true;
}


void OverworldItem::Render()
{
	Engine::Instance().m_render->painter().RenderBox(position,{16,16},{1,1},{0.5f,0.5f}, true, {255,255,255,255});
}

void OverworldItem::Initialize(Item item, int q, Vector2 position)
{
	id = item.id;
	//texture = 
	quantity = q;
	this->position = position;

	body->SetPhysicPosition(this->position.x, this->position.y);
}

void OverworldItem::PickUp()
{
	Pooling::Instance().ReturnObject(this);
}

void OverworldItem::InitPoolObject()
{
	Engine::Instance().m_render->AddToRenderQueue(*this, *this);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");

	///Crear Collider y sensor

	b2FixtureUserData sensorData;
	sensorData.pointer = (uintptr_t)(&interactionSensor); 
	body = Engine::Instance().m_physics->factory().CreateBox({ 0,0 }, 0.8f, 0.8f, sensorData);
	body->SetType(PhysBody::BodyType::Kinematic);
	body->SetSensor(0, true);

	ModulePhysics::Layer category, mask;
	category.flags.trigger_layer = 1;
	mask.flags.player_layer = 1;
	body->SetFilter(0, category.rawValue, mask.rawValue, 0);
	
	interactionSensor.SetFixtureToTrack(body, 0);
	interactionSensor.AcceptOnlyTriggers(false);
}

void OverworldItem::ResetPoolObject()
{
	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_render->RemoveFomRenderQueue(*this);
	Reset();
	delete body;
	/// Destruir collider
}


