#include "Character.h"
#include "FollowerCharacter.h"
#include "ModulePhysics.h"
#include "ModuleTime.h"

Character::Character()
{

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
