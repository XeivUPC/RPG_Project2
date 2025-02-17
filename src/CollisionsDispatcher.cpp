#include "CollisionsDispatcher.h"
#include "CollisionSensor.h"

CollisionsDispatcher::CollisionsDispatcher()
{

}

CollisionsDispatcher::~CollisionsDispatcher()
{
   
}

void CollisionsDispatcher::BeginContact(b2Contact* contact)
{

    b2FixtureUserData dataA = contact->GetFixtureA()->GetUserData();
    CollisionSensor* sensorA = (CollisionSensor*)dataA.pointer;
    if (sensorA != nullptr)
        sensorA->BeginContact(contact);

    b2FixtureUserData dataB = contact->GetFixtureB()->GetUserData();
    CollisionSensor* sensorB = (CollisionSensor*)dataB.pointer;
    if (sensorB != nullptr)
        sensorB->BeginContact(contact);

}

void CollisionsDispatcher::EndContact(b2Contact* contact)
{

    b2FixtureUserData dataA = contact->GetFixtureA()->GetUserData();
    CollisionSensor* sensorA = (CollisionSensor*)dataA.pointer;
    if (sensorA != nullptr)
        sensorA->EndContact(contact);

    b2FixtureUserData dataB = contact->GetFixtureB()->GetUserData();
    CollisionSensor* sensorB = (CollisionSensor*)dataB.pointer;
    if (sensorB != nullptr)
        sensorB->EndContact(contact);
}
