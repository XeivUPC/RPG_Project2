#include "OverworldItem.h"
#include "ModuleAssetDatabase.h"
#include "Engine.h"
#include "ModuleUpdater.h"
#include "ModuleRender.h"
#include "Pooling.h"
#include "Item.h"
#include "TextureAtlas.h"
#include "DrawingTools.h"

#include "GameScene.h"
#include "PlayerCharacter.h"
#include "Inventory.h"

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
	Engine::Instance().m_render->painter().RenderTexture(*texture,position,&rectangle,{1,1},0,{0.5f,0.5f});
}

void OverworldItem::Initialize(Item* item, int q, Vector2 position)
{
	itemRef = item;

	TextureAtlas* atlas = Engine::Instance().m_assetsDB->GetAtlas("items_atlas");
	texture = atlas->texture;
	rectangle = atlas->sprites[itemRef->id].rect;

	quantity = q;
	this->position = position;

	body->SetPhysicPosition(this->position.x, this->position.y);
}

void OverworldItem::PickUp()
{
	int added = Engine::Instance().s_game->GetPlayer()->inventory->AddItem(*(new InventoryItem(itemRef)),quantity);
	quantity -= added;
	if (quantity <= 0) {
		Pooling::Instance().ReturnObject(this);
	}
	Engine::Instance().s_game->GetPlayer()->inventory->displayInventory();
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
	Engine::Instance().m_render->RemoveFromRenderQueue(*this);
	Reset();
	delete body;
	/// Destruir collider
}


