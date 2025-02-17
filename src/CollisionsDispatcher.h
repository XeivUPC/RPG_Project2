#pragma once
#include "ModulePhysics.h"
#include <vector>

class CollisionSensor;

class CollisionsDispatcher : public b2ContactListener {
public:
    CollisionsDispatcher();
    ~CollisionsDispatcher(); 

private:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact)override;
};
