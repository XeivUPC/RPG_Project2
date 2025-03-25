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
	SetNpcId(_npcId);
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

	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	Engine::Instance().m_render->RemoveFomRenderQueue(*this);
	return true;
}

void FollowerCharacter::SetNpcId(int _npcId)
{
	npcId = _npcId;
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
			if(position != characterToFollow->pathFollowersData[i])
				moveDirection = Vector2::Direction(position, characterToFollow->pathFollowersData[i]);

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

