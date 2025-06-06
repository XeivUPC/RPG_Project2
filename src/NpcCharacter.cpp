#include "NpcCharacter.h"
#include "Pooling.h"

#include "Engine.h"
#include "ModuleRender.h"
#include "GameScene.h"
#include "ModuleAssetDataBase.h"
#include "ModuleUpdater.h"
#include "ModuleInput.h"
#include "DrawingTools.h"
#include "ModulePhysics.h"
#include "ModuleDebug.h"
#include "PhysicFactory.h"
#include "Animator.h"
#include "AnimationClip.h"

NpcCharacter::NpcCharacter()
{
	baseSpeed = 2.5f;

    renderLayer = 3;
    renderOffsetSorting = { 0,2 };
}

NpcCharacter::~NpcCharacter()
{
	if (animator!=nullptr) {
		animator->CleanUp();
		delete animator;
	}
	
}

bool NpcCharacter::PreUpdate()
{
	SearchPath();
	return true;
}

bool NpcCharacter::Update()
{
	Animate();
    Move();
	animator->clip()->UpdateClip();

	Character::Update();

    return true;
}

void NpcCharacter::Render()
{
	animator->clip()->RenderClip();

	if (!Engine::Instance().m_debug->godmode)
		return;
	if (path.size() > 1)
	{
		int pathSize = int(path.size() - 1);
		if (movementType == MovementType::Loop)
			pathSize = int(path.size());
		for (size_t i = 0; i < pathSize; i++)
		{
			if (i == path.size() - 1)
				Engine::Instance().m_render->painter().RenderLine(path[i], path[0], { 255,255,255,255 });
			else
				Engine::Instance().m_render->painter().RenderLine(path[i], path[i + 1], { 255,255,255,255 });
		}
	}
}

bool NpcCharacter::CleanUp()
{
	
    Pooling::Instance().ReturnObject(this);
    return true;
}

void NpcCharacter::SetNpcPath(vector<Vector2> _path, MovementType _movementType)
{
	path = _path;
	movementType = _movementType;
}

void NpcCharacter::SetDialoguePath(string _dialoguePath)
{
	dialogPath = _dialoguePath;
}

bool NpcCharacter::SetCharacterId(string _charId)
{
	if (Character::SetCharacterId(_charId)) {

		if(characterData->dialogue!="")
			SetDialoguePath(characterData->dialogue);

		texture = Engine::Instance().m_assetsDB->GetTexture(characterData->charTemplate->textureId);

		for (auto& animClip : animator->GetAnimationClips()) {
			for (auto& sprite : animClip.GetSprites()) {
				sprite.SetTexture(texture);
			}
		}

		return true;
	}
	return false;
}


void NpcCharacter::SearchPath()
{
	if (path.size() <= 1) {
		moveDirection = { 0, 0 };
	}
	else if (Vector2::Approximately(position, path[pathPosition], 2)) {
		int nextTarget = pathPosition + pathDirection;
		bool isNextTargetValid = (nextTarget >= 0 && nextTarget < path.size());

		if (!isNextTargetValid) {
			switch (movementType) {
			case MovementType::PingPong:
				pathDirection *= -1;
				nextTarget = pathPosition + pathDirection;
				SetPosition(path[pathPosition]);
				isNextTargetValid = true;
				break;

			case MovementType::Loop:
				if (nextTarget >= path.size()) {
					nextTarget = 0;
				}
				else {
					nextTarget = (int)(path.size() - 1);
				}
				isNextTargetValid = true;
				break;

			case MovementType::StopAtEnd:
				moveDirection = { 0, 0 };
				pathPosition = (pathDirection > 0) ? (int)(path.size() - 1) : 0;
				SetPosition(path[pathPosition]);
				return;
			}
		}

		if (isNextTargetValid) {
			pathPosition = nextTarget;
			moveDirection = Vector2::Direction(position, path[pathPosition]);
		}
	}
	else if (moveDirection == Vector2{0,0}) {
		moveDirection = Vector2::Direction(position, path[pathPosition]);
	}

}

void NpcCharacter::Move()
{
	body->SetVelocity(moveDirection * baseSpeed * speedModifier);
	position = body->GetPhysicPosition();
}

void NpcCharacter::Animate()
{

	bool isMoving = (moveDirection != Vector2{ 0,0 });
	if (isMoving)
		lastDirection = moveDirection;

	Vector2 animationDirection = lastDirection;

	if (moveDirection.magnitude() != 0)
		moveDirection.normalize();

	


	string animationId = isMoving ? (speedModifier == runSpeedModifier ? "run-" : "walk-") : "idle-";
	string animationDirectionId = "";

	if (std::abs(animationDirection.x) > 0.2f) {

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

	bool flip = animationDirection.x < 0;
	animator->clip()->Flip(flip);
}


void NpcCharacter::Interact(Vector2 from)
{
	if (UseDialogue()) {
		moveDirection = { 0,0 };
		Vector2 direction = Vector2::Direction(position, from);
		lastDirection = direction;
		Animate();
		animator->clip()->UpdateClip();
	}
	else {

	}
}

bool NpcCharacter::UseDialogue()
{
	if (dialogPath == "")
		return false;
	Engine::Instance().s_game->SetDialogue(dialogPath);
	Engine::Instance().s_game->SetState(GameScene::State::Dialogue);
	return true;
}


void NpcCharacter::InitPoolObject()
{
    Engine::Instance().m_render->AddToRenderQueue(*this, *this);
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");

    body = Engine::Instance().m_physics->factory().CreateBox({ 0,0.2f }, 0.5f, 0.2f);
    body->SetType(PhysBody::BodyType::Kinematic);
    body->SetFriction(0, 0);
    body->SetFixedRotation(true);
	ModulePhysics::Layer category, mask;
	category.flags.npc_layer = 1;
	mask.flags.player_layer = 1;
	body->SetFilter(0, category.rawValue, mask.rawValue, 0);

	category.rawValue = 0;
	mask.rawValue = 0;

    body->data = (uintptr_t)((IInteractuable*)this);
    int fixtureIndex = Engine::Instance().m_physics->factory().AddCircle(body, { 0,0.1f }, 1.0f);
    body->SetSensor(fixtureIndex, true);
    category.flags.interactable_layer = 1;
    mask.flags.interactable_layer = 1;
    body->SetFilter(fixtureIndex, category.rawValue, mask.rawValue, 0);
}

void NpcCharacter::ResetPoolObject()
{
	moveDirection = { 0,0 };
	lastDirection = { 0,1 };

	pathPosition = 0;
	pathDirection = 1;
	path.clear();

    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_render->RemoveFromRenderQueue(*this);

	Character::CleanUp();
    delete body;
}
