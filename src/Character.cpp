#include "Character.h"
#include "ModulePhysics.h"

Character::Character()
{
}

Character::~Character()
{
}

bool Character::Update()
{
    return true;
}

void Character::Render()
{
}

bool Character::CleanUp()
{
    return true;
}

void Character::SetPosition(Vector2 newPosition)
{
    position = newPosition;
    if (body != nullptr)
        body->SetPhysicPosition(position.x, position.y);
}
