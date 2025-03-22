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
#include "PhysicFactory.h"
#include "Animator.h"
#include "AnimationClip.h"

NpcCharacter::NpcCharacter()
{
    texture = Engine::Instance().m_assetsDB->GetTexture("npc_test");

    renderLayer = 3;
    renderOffsetSorting = { 0,2 };

	int spriteSize = 64;
	animator = new Animator
	(
		{
			AnimationClip("idle-down", true, false, 0.1f,
			{
				Sprite(texture, {0 * spriteSize,0 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {1 * spriteSize,0 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {2 * spriteSize,0 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {3 * spriteSize,0 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f })
			},&position,&scale),
			AnimationClip("idle-horizontally", true, false, 0.1f,
			{
				Sprite(texture, {0 * spriteSize,1 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {1 * spriteSize,1 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {2 * spriteSize,1 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {3 * spriteSize,1 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f })
			},&position,&scale),
			AnimationClip("idle-top", true, false, 0.1f,
			{
				Sprite(texture, {0 * spriteSize,2 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {1 * spriteSize,2 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {2 * spriteSize,2 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {3 * spriteSize,2 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f })
			},&position,&scale),
			AnimationClip("walk-down", true, false, 0.1f,
			{
				Sprite(texture, {0 * spriteSize,3 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {1 * spriteSize,3 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {2 * spriteSize,3 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {3 * spriteSize,3 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {4 * spriteSize,3 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {5 * spriteSize,3 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f })
			},&position,&scale),
			AnimationClip("walk-horizontally", true, false, 0.1f,
			{
				Sprite(texture, {0 * spriteSize,4 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {1 * spriteSize,4 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {2 * spriteSize,4 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {3 * spriteSize,4 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {4 * spriteSize,4 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {5 * spriteSize,4 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f })
			},&position,&scale),
			AnimationClip("walk-top", true, false, 0.1f,
			{
				Sprite(texture, {0 * spriteSize,5 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {1 * spriteSize,5 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {2 * spriteSize,5 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {3 * spriteSize,5 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {4 * spriteSize,5 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {5 * spriteSize,5 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f })
			},&position,&scale)

		}, 0
	);
}

NpcCharacter::~NpcCharacter()
{
	animator->CleanUp();
	delete animator;
}

bool NpcCharacter::Update()
{
    SearchPath();
    Move();

	animator->clip()->UpdateClip();
    return true;
}

void NpcCharacter::Render()
{
    
    SDL_Rect rect = { 0,0,64,64 };

    //Engine::Instance().m_render->painter().RenderTexture(*texture, position, &rect, { 1.f,1.f }, 0, { 0.5f,0.75f });
	animator->clip()->RenderClip();

}

bool NpcCharacter::CleanUp()
{
	
    Pooling::Instance().ReturnObject(this);
    return true;
}

void NpcCharacter::SetNpcData(int _npcId, Vector2 _position)
{
    npcId = _npcId;
    SetPosition(position);
}

void NpcCharacter::SearchPath()
{
}

void NpcCharacter::Move()
{
    position = body->GetPhysicPosition();

	bool isMoving = (moveDirection != Vector2{ 0,0 });
	if (isMoving)
		lastDirection = moveDirection;

	Vector2 animationDirection = lastDirection;

	if (moveDirection.magnitude() != 0)
		moveDirection.normalize();

	bool flip = animationDirection.x < 0;
	animator->clip()->Flip(flip);
	if (std::abs(animationDirection.x) >= std::abs(animationDirection.y)) {

		if (isMoving) {
			animator->Animate("walk-horizontally");
		}
		else {
			animator->Animate("idle-horizontally");
		}
	}
	else if (animationDirection.y > 0) {
		if (isMoving) {
			animator->Animate("walk-down");
		}
		else {
			animator->Animate("idle-down");
		}
	}
	else {
		if (isMoving) {
			animator->Animate("walk-top");
		}
		else {
			animator->Animate("idle-top");
		}
	}
}

void NpcCharacter::Interact()
{
    printf("Ey\n");
    Engine::Instance().s_game->SetState(GameScene::State::Dialogue);
    Engine::Instance().s_game->SetDialogue("Assets/Dialogues/test2.json");
}

void NpcCharacter::InitPoolObject()
{
    Engine::Instance().m_render->AddToRenderQueue(*this, *this);
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");

    body = Engine::Instance().m_physics->factory().CreateBox({ 0,0.2f }, 0.5f, 0.2f);
    body->SetType(PhysBody::BodyType::Kinematic);
    body->SetFriction(0, 0);
    body->SetFixedRotation(true);

    body->data = (uintptr_t)((IInteractuable*)this);

    int fixtureIndex = Engine::Instance().m_physics->factory().AddCircle(body, { 0,0.1f }, 1.0f);
    body->SetSensor(fixtureIndex, true);
    ModulePhysics::Layer category, mask;
    category.flags.interactable_layer = 1;
    mask.flags.interactable_layer = 1;
    body->SetFilter(fixtureIndex, category.rawValue, mask.rawValue, 0);


   
}

void NpcCharacter::ResetPoolObject()
{
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_render->RemoveFomRenderQueue(*this);
    delete body;
}
