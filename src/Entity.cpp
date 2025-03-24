#include "Entity.h"
#include "Engine.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

bool Entity::Init()
{
    return true;
}

bool Entity::Start()
{

    return true;
}

void Entity::Render()
{

}

bool Entity::PreUpdate()
{
    return true;
}

bool Entity::Update()
{
    return true;
}

bool Entity::PostUpdate()
{
    return true;
}

bool Entity::CleanUp()
{

    return true;
}


void Entity::SetPosition(Vector2 newPosition)
{
    position = newPosition;
}

Vector2 Entity::GetPosition()
{
    return position;
}

void Entity::SetAngle(double newAngle)
{
    angle = newAngle;
}

void Entity::SetScale(float newScale)
{
    scale = newScale;
}

double Entity::GetAngle()
{
    return angle;
}

float Entity::GetScale()
{
    return scale;
}

void Entity::SetAnchor(Vector2 newAnchor)
{
    anchor = newAnchor;
}

Vector2 Entity::GetAnchor()
{
    return anchor;
}
