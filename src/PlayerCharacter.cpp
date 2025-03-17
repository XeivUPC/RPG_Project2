#include "PlayerCharacter.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleUpdater.h"
#include "ModuleInput.h"
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

	body = Engine::Instance().m_physics->factory().CreateBox({0,0.2f},0.5f,0.2f);
	body->SetFriction(0, 0);
	body->body->SetFixedRotation(true);
}

PlayerCharacter::~PlayerCharacter()
{
}

bool PlayerCharacter::Update()
{
	GetInput();
	Move();
	return true;
}

void PlayerCharacter::Render()
{
	Engine::Instance().m_render->painter().RenderBox(body->GetPhysicPosition(), { 20,50 }, { 1,1 }, {0.5f,1 }, true);
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
