#include "FollowerCharacter.h"

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

FollowerCharacter::FollowerCharacter(Character* _characterToFollow, float _delayDistance, int _npcId)
{
	SetCharacterToFollow(_characterToFollow);
	SetCharacterId(_npcId);
	delayDistance = _delayDistance;
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
			},&position,&scale),
				AnimationClip("run-down", true, false, 0.1f,
			{
				Sprite(texture, {0 * spriteSize,6 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {1 * spriteSize,6 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {2 * spriteSize,6 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {3 * spriteSize,6 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {4 * spriteSize,6 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {5 * spriteSize,6 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f })
			},&position,&scale),
			AnimationClip("run-horizontally", true, false, 0.1f,
			{
				Sprite(texture, {0 * spriteSize,7 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {1 * spriteSize,7 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {2 * spriteSize,7 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {3 * spriteSize,7 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {4 * spriteSize,7 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {5 * spriteSize,7 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f })
			},&position,&scale),
			AnimationClip("run-top", true, false, 0.1f,
			{
				Sprite(texture, {0 * spriteSize,8 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {1 * spriteSize,8 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {2 * spriteSize,8 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {3 * spriteSize,8 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {4 * spriteSize,8 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f }),
				Sprite(texture, {5 * spriteSize,8 * spriteSize,spriteSize,spriteSize},{0.5f,0.75f })
			},&position,&scale)

		}, 0
	);

	Engine::Instance().m_render->AddToRenderQueue(*this, *this);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");
}



FollowerCharacter::~FollowerCharacter()
{
	animator->CleanUp();
	delete animator;
}

bool FollowerCharacter::Update()
{
	
	SearchPath();
	Animate();
	Move();

	animator->clip()->UpdateClip();

	Character::Update();
	return true;
}

void FollowerCharacter::Render()
{

	SDL_Rect rect = { 0,0,64,64 };

	//Engine::Instance().m_render->painter().RenderTexture(*texture, position, &rect, { 1.f,1.f }, 0, { 0.5f,0.75f });
	animator->clip()->RenderClip();
}

bool FollowerCharacter::CleanUp()
{
	Character::CleanUp();
	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_render->RemoveFomRenderQueue(*this);
	return true;
}


void FollowerCharacter::SetCharacterToFollow(Character* _characterToFollow)
{
	characterToFollow = _characterToFollow;
}

void FollowerCharacter::SearchPath()
{
	float accumulator = 0;

	for (int i = 0; i < (int)(characterToFollow->pathFollowersData.size())-1; i++) {
		float previousDistance = accumulator;
		accumulator += Vector2::Distance(characterToFollow->pathFollowersData[i], characterToFollow->pathFollowersData[i+1]);
		if (delayDistance < accumulator) {
			float percentaje = 100 - (delayDistance - previousDistance) *100 / (accumulator- previousDistance);

			position = Vector2::Lerp(characterToFollow->pathFollowersData[i+1], characterToFollow->pathFollowersData[i], percentaje/100.f);
			if (position != characterToFollow->pathFollowersData[i]) {
				moveDirection = Vector2::Direction(characterToFollow->pathFollowersData[i + 1], characterToFollow->pathFollowersData[i]);
			}

			if (characterToFollow->moveDirection == Vector2{0,0})
				moveDirection = { 0,0 };

			break;
		}
	}
}
void FollowerCharacter::Move()
{
}

void FollowerCharacter::Animate()
{

	bool isMoving = (moveDirection != Vector2{ 0,0 });
	if (isMoving)
		lastDirection = moveDirection;

	Vector2 animationDirection = lastDirection;

	if (moveDirection.magnitude() != 0)
		moveDirection.normalize();

	bool flip = animationDirection.x < 0;
	animator->clip()->Flip(flip);


	string animationId = isMoving ? (characterToFollow->speedModifier == characterToFollow->runSpeedModifier ? "run-" : "walk-") : "idle-";
	string animationDirectionId = "";

	if (std::abs(animationDirection.x) >= 0.5f) {

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

