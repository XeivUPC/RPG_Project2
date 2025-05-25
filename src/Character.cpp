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

		for (size_t i = 0; i < followers.size(); i++)
		{
			followers[i]->Update();
		}
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

bool Character::SetCharacterId(string _charId)
{
    if (_charId != characterId)
    {
        characterId = _charId;
        characterData = &CharacterDatabase::Instance().GetCharacterDefinition(characterId);
        return true;
    }
    return false;
}

string Character::GetCharacterId() const
{
	return characterId;
}

void Character::ClearFollowerPath()
{
	pathFollowersData.clear();
}

Animator& Character::GetAnimator() const
{
	return *animator;
}

bool Character::AddFollower(string _charId, float distance)
{
	if (followers.size() >= maxFollowers)
		return false;

	followers.emplace_back(new FollowerCharacter(this, totalFollowerDistance + distance, _charId));
	currentFollowers++;
	totalFollowerDistance += distance;
	return true;
}

bool Character::RemoveFollowerById(string _charId)
{
	int charPos = 0;
	for (auto it = followers.begin(); it != followers.end(); ++it) {
		if ((*it)->GetCharacterId() == _charId) {
			int distanceDifference = (int)((*it)->GetDelayDistance() - (charPos > 0 ? followers[charPos - 1]->GetDelayDistance() : 0));
			totalFollowerDistance -= distanceDifference;
			for (int i = charPos + 1; i < followers.size(); ++i) {
				followers[i]->SetDelayDistance(followers[i]->GetDelayDistance() - distanceDifference);
			}
			followers.erase(it);

			(*it)->CleanUp();
			delete (*it);

			currentFollowers--;
			return true;
		}
		charPos++;
	}
	return false;
}

bool Character::RemoveFollowerByIndex(int followerPos)
{
	if (followerPos < (int)followers.size()) {
		int distanceDifference = (int)(followers[followerPos]->GetDelayDistance() - (followerPos > 0 ? followers[followerPos - 1]->GetDelayDistance() : 0));
		totalFollowerDistance -= distanceDifference;
		for (int i = followerPos + 1; i < followers.size(); ++i) {
			followers[i]->SetDelayDistance(followers[i]->GetDelayDistance() - distanceDifference);
		}

		followers[followerPos]->CleanUp();
		delete (followers[followerPos]);
		followers.erase(followers.begin() + followerPos);

		currentFollowers--;
		return true;
	}
	return false;
}

bool Character::EditFollower(string _charId, int _charIndex)
{
	if (_charIndex < followers.size()) {
		followers[_charIndex]->SetCharacterId(_charId);
		return true;
	}
	return false;
}

bool Character::GetFollowers() const
{
	return true;
	return false;
}

void Character::SetFollowers(vector<string> ids, float distance)
{
	for (size_t i = followers.size(); i < ids.size(); i++) {
		AddFollower(ids[(int)i], distance);
	}

	for (int i = 0; i < followers.size(); i++)
	{
		if (ids.size() <= i) {
			//Remove
			RemoveFollowerByIndex(i);
		}
		else {
			//Edit
			EditFollower(ids[i], i);
		}
	}
}
