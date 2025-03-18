#include "PlayerCharacter.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleAssetDataBase.h"
#include "ModuleUpdater.h"
#include "ModuleInput.h"
#include "ModuleTime.h"
#include "DrawingTools.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"

PlayerCharacter::PlayerCharacter()
{
	characterName = "Cassian";
	Engine::Instance().m_render->AddToRenderQueue(*this, *this);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");
	renderLayer = 3;
	renderOffsetSorting = { 0,2 };


	b2FixtureUserData sensorData;
	sensorData.pointer = (uintptr_t)(&interactionSensor);
	body = Engine::Instance().m_physics->factory().CreateBox({0,0.2f},0.5f,0.2f, sensorData);
	body->SetFriction(0, 0);
	body->body->SetFixedRotation(true);
	ModulePhysics::Layer category;
	ModulePhysics::Layer mask;
	category.flags.interactable_layer = 1;
	category.flags.player_layer = 1;
	mask.flags.interactable_layer = 1;
	mask.flags.default_layer = 1;
	body->SetFilter(0, category.rawValue, mask.rawValue, 0);

	interactionSensor.SetFixtureToTrack(body, 0);

	texture = Engine::Instance().m_assetsDB->GetTexture("player_test");
}

PlayerCharacter::~PlayerCharacter()
{
}

bool PlayerCharacter::Update()
{
	previousPhysicsPosition = position;

	if (interactionSensor.IsBeingTriggered()) {
		printf("interacting\n");
		PhysBody* interactor = interactionSensor.GetClosestBodyColliding();
		/// Call Interact
		 
	}

	GetInput();
	Move();
	return true;
}

void PlayerCharacter::Render()
{
	float alpha = Engine::Instance().m_time->GetPhysicsInterpolationAlpha();
	
	Vector2 renderPosition = Vector2::Lerp(previousPhysicsPosition, position, alpha);

	SDL_Rect rect = { 0,0,64,64 };

	Engine::Instance().m_render->painter().RenderTexture(*texture,renderPosition, &rect, { 1.f,1.f },0, {0.5f,0.75f });
}

bool PlayerCharacter::CleanUp()
{
	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_render->RemoveFomRenderQueue(*this);
	delete body;
	return true;
}

void PlayerCharacter::GetInput()
{
	//// Process Direction

	moveDirection = { 0,0 };
	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		moveDirection.y -= 1;
	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		moveDirection.y += 1;
	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		moveDirection.x -= 1;
	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		moveDirection.x += 1;

	if(moveDirection.magnitude()!=0)
		moveDirection.normalize();
}

void PlayerCharacter::Move()
{
	body->SetVelocity(moveDirection * speed);
	position = body->GetPhysicPosition();
}
