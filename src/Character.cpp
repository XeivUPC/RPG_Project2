#include "Character.h"
#include "FollowerCharacter.h"
#include "ModulePhysics.h"
#include "ModuleTime.h"

#include "Animator.h"
#include "AnimationClip.h"

Character::Character()
{
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
}

Character::~Character()
{
}

bool Character::Update()
{
    if (followers.size() > 0) {
        updateFollowerPathTimer.Step(ModuleTime::deltaTime);
        if (moveDirection != Vector2{ 0,0 }) {
            if (updateFollowerPathTimer.ReadSec() > followerPathSmoothing) {
                updateFollowerPathTimer.Start();
                if (pathFollowersData.size() >= maxFollowersPathDistance) {
                    pathFollowersData.pop_back();
                }
                pathFollowersData.push_front(position);
            }
        }

        if (pathFollowersData.size() == 0)
            pathFollowersData.push_front(position);
        else
            pathFollowersData[0] = position;
    }
    return true;
}

void Character::Render()
{
}

bool Character::CleanUp()
{
    for (int i = 0; i < followers.size(); i++) {
        followers[i]->CleanUp();
        delete followers[i];
    }
    followers.clear();
    return true;
}

void Character::SetPosition(Vector2 newPosition)
{
    position = newPosition;
    if (body != nullptr)
        body->SetPhysicPosition(position.x, position.y);
}

bool Character::SetCharacterId(int _charId)
{
    if (_charId != characterId)
    {
        characterId = _charId;
        characterData = &CharacterDatabase::Instance().GetCharacterData(characterId);
        return true;
    }
    return false;
}
