#include "Bird.h"
#include "BirdFlock.h"
#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleRender.h"
#include "ModuleTime.h"
#include "Animator.h"
#include "AnimationClip.h"
#include "Pooling.h"

#include <random>

Bird::Bird()
{
	int spriteSize = 16;
	SDL_Texture* texture = Engine::Instance().m_assetsDB->GetTexture("birds");

	std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution<> dis(0, 6);

	int type = dis(gen);
	animator = new Animator
	(
		{
			AnimationClip("idle", true, false, 0.1f,
			{
				Sprite(texture, {0 * spriteSize,type * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
				Sprite(texture, {1 * spriteSize,type * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
				Sprite(texture, {2 * spriteSize,type * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
				Sprite(texture, {3 * spriteSize,type * spriteSize,spriteSize,spriteSize},{0.5f,0.5f })
			},nullptr, nullptr),
			AnimationClip("fly", true, false, 0.1f,
			{
				Sprite(texture, {4 * spriteSize,type * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
				Sprite(texture, {5 * spriteSize,type * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
				Sprite(texture, {6 * spriteSize,type * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
				Sprite(texture, {7 * spriteSize,type * spriteSize,spriteSize,spriteSize},{0.5f,0.5f })
			},nullptr, nullptr),

		}, 0
	);
}

Bird::~Bird()
{
	delete animator;
}

void Bird::UpdateBird()
{
	if (scared) {
		//// MoveLogic
		position += moveDirection * speed * (float)ModuleTime::deltaTime;
		if (!Engine::Instance().m_render->IsCircleCameraVisible(position, 18)) {
			flock->RemoveBird(this);
		}
		
	}
	animator->clip()->UpdateClip();
}

void Bird::RenderBird()
{
	animator->clip()->RenderClip(position);
}

void Bird::Scare(Vector2 positionFromUser)
{
	animator->Animate("fly");



	moveDirection = Vector2::Direction(positionFromUser, position);
	animator->clip()->Flip(moveDirection.x < 0);
	scared = true;
}

void Bird::Initialize(BirdFlock* _flock, Vector2 _position)
{
	flock = _flock;
	position = _position;
	animator->Animate("idle");


	std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution<> dis(0, 1);

	animator->clip()->Flip(dis(gen));
}

void Bird::InitPoolObject()
{
	scared = false;
}

void Bird::ResetPoolObject()
{
	
}
