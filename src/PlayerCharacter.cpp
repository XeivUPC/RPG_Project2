#include "PlayerCharacter.h"
#include "FollowerCharacter.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleAssetDataBase.h"
#include "ModuleUpdater.h"
#include "ModuleInput.h"
#include "ModuleTime.h"
#include "DrawingTools.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"
#include "IInteractuable.h"
#include "Animator.h"
#include "AnimationClip.h"
#include "AudioContainer.h"
#include "Party.h"

PlayerCharacter::PlayerCharacter()
{
	baseSpeed = 4;

	Engine::Instance().m_render->AddToRenderQueue(*this, *this);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");
	renderLayer = 3;
	renderOffsetSorting = { 0,2 };


	b2FixtureUserData sensorData;
	sensorData.pointer = (uintptr_t)(&interactionSensor);
	body = Engine::Instance().m_physics->factory().CreateBox({0,0.2f},0.5f,0.2f);
	body->SetFriction(0, 0);
	body->SetFixedRotation(true);
	ModulePhysics::Layer category, mask;
	category.flags.player_layer = 1;
	mask.flags.trigger_layer = 1;
	mask.flags.ground_layer = 1;
	mask.flags.npc_layer = 1;
	body->SetFilter(0, category.rawValue, mask.rawValue, 0);

	category.rawValue = 0;
	mask.rawValue = 0;

	int fixtureIndex = Engine::Instance().m_physics->factory().AddBox(body, { 0,0 }, 0.5f, 0.5f, sensorData);
	interactionSensor.SetFixtureToTrack(body, fixtureIndex);
	body->SetSensor(fixtureIndex, true);
	category.flags.interactable_layer = 1;
	mask.flags.interactable_layer = 1;
	body->SetFilter(fixtureIndex, category.rawValue, mask.rawValue, 0);

	SetCharacterId(0);
	party = new Party(0);
	party->onPartyChanged.Subscribe([this]() {SetFollowers(party->GetActiveFollowersIds(), distanceBetweenFollowers); });
	party->onPartyChanged.Subscribe([this]() {SetCharacterId(party->GetLeaderId()); });

	ModuleAudio* audioRef = Engine::Instance().m_audio;
	ModuleAssetDatabase* assetsRef = Engine::Instance().m_assetsDB;
	AudioContainer* footstepContainer = assetsRef->GetAudioContainer("footsteps_container");

	animator->GetAnimationClip("walk-top")->GetSprite(0).onSpriteSelected.Subscribe([this,audioRef, assetsRef, footstepContainer]() {audioRef->PlaySFX(footstepContainer->GetNextClip()); });
	animator->GetAnimationClip("walk-top")->GetSprite(3).onSpriteSelected.Subscribe([this, audioRef, assetsRef, footstepContainer]() {audioRef->PlaySFX(footstepContainer->GetNextClip()); });

	animator->GetAnimationClip("walk-down")->GetSprite(0).onSpriteSelected.Subscribe([this,audioRef, assetsRef, footstepContainer]() {audioRef->PlaySFX(footstepContainer->GetNextClip()); });
	animator->GetAnimationClip("walk-down")->GetSprite(3).onSpriteSelected.Subscribe([this,audioRef, assetsRef, footstepContainer]() {audioRef->PlaySFX(footstepContainer->GetNextClip()); });

	animator->GetAnimationClip("walk-horizontally")->GetSprite(0).onSpriteSelected.Subscribe([this,audioRef, assetsRef, footstepContainer]() {audioRef->PlaySFX(footstepContainer->GetNextClip()); });
	animator->GetAnimationClip("walk-horizontally")->GetSprite(3).onSpriteSelected.Subscribe([this,audioRef, assetsRef, footstepContainer]() {audioRef->PlaySFX(footstepContainer->GetNextClip()); });

	animator->GetAnimationClip("run-top")->GetSprite(0).onSpriteSelected.Subscribe([this, audioRef, assetsRef, footstepContainer]() {audioRef->PlaySFX(footstepContainer->GetNextClip()); });
	animator->GetAnimationClip("run-top")->GetSprite(3).onSpriteSelected.Subscribe([this, audioRef, assetsRef, footstepContainer]() {audioRef->PlaySFX(footstepContainer->GetNextClip()); });

	animator->GetAnimationClip("run-down")->GetSprite(0).onSpriteSelected.Subscribe([this, audioRef, assetsRef, footstepContainer]() {audioRef->PlaySFX(footstepContainer->GetNextClip()); });
	animator->GetAnimationClip("run-down")->GetSprite(3).onSpriteSelected.Subscribe([this, audioRef, assetsRef, footstepContainer]() {audioRef->PlaySFX(footstepContainer->GetNextClip()); });

	animator->GetAnimationClip("run-horizontally")->GetSprite(0).onSpriteSelected.Subscribe([this, audioRef, assetsRef, footstepContainer]() {audioRef->PlaySFX(footstepContainer->GetNextClip()); });
	animator->GetAnimationClip("run-horizontally")->GetSprite(3).onSpriteSelected.Subscribe([this, audioRef, assetsRef, footstepContainer]() {audioRef->PlaySFX(footstepContainer->GetNextClip()); });

}

PlayerCharacter::~PlayerCharacter()
{
}

bool PlayerCharacter::Update()
{
	previousPhysicsPosition = position;

	GetInput();
	Animate();
	Move();

	animator->clip()->UpdateClip();

	Character::Update();
	return true;
}

void PlayerCharacter::Render()
{
	float alpha = Engine::Instance().m_time->GetPhysicsInterpolationAlpha();
	
	Vector2 renderPosition = Vector2::Lerp(previousPhysicsPosition, position, alpha);

	animator->clip()->RenderClip();
}

bool PlayerCharacter::CleanUp()
{
	Character::CleanUp();
	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_render->RemoveFomRenderQueue(*this);
	delete body;

	delete party;

	animator->CleanUp();
	delete animator;
	return true;
}

bool PlayerCharacter::SetCharacterId(int _charId)
{

	if (Character::SetCharacterId(_charId)) {
		texture = Engine::Instance().m_assetsDB->GetTexture(characterData->textureId);

		for (auto& animClip : animator->GetAnimationClips()) {
			for (auto& sprite : animClip.GetSprites()) {
				sprite.SetTexture(texture);
			}
		}
		return true;
	}
	return false;
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

	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		speedModifier = runSpeedModifier;
	}else{
		speedModifier = 1;
	}


	///  Process Actions

	if (interactionSensor.IsBeingTriggered()) {

		if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			PhysBody* interactor = interactionSensor.GetClosestBodyColliding();
			IInteractuable* interactuableObj = reinterpret_cast<IInteractuable*>(interactor->data);

			moveDirection = { 0,0 };
			interactuableObj->Interact();
		}

	}

	/// Party Testing

	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {
		party->AddFollower(1);
	}

	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_H) == KEY_DOWN) {
		party->RemoveFollower(1);
	}

	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		party->AddFollower(2);
	}

	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {
		party->RemoveFollower(2);
	}

}

void PlayerCharacter::Move()
{
	body->SetVelocity(moveDirection * baseSpeed * speedModifier);
	position = body->GetPhysicPosition();
}

void PlayerCharacter::Animate()
{
	bool isMoving = (moveDirection != Vector2{ 0,0 });
	if (isMoving)
		lastDirection = moveDirection;

	Vector2 animationDirection = lastDirection;

	if (moveDirection.magnitude() != 0)
		moveDirection.normalize();

	bool flip = animationDirection.x < 0;
	animator->clip()->Flip(flip);


	string animationId = isMoving ? (speedModifier == runSpeedModifier ? "run-" : "walk-") : "idle-";
	string animationDirectionId = "";

	if (std::abs(animationDirection.x) >= std::abs(animationDirection.y)) {

		animationDirectionId = "horizontally";
	}
	else if (animationDirection.y > 0) {
		animationDirectionId = "down";
	}
	else {
		animationDirectionId = "top";
	}
	animationId += animationDirectionId;
	animator->Animate(animationId);
}
