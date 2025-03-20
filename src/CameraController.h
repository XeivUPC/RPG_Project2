#pragma once
#include "Entity.h"

class CameraController : public Entity {
public:
    CameraController();
    ~CameraController();

    bool PostUpdate();
    bool CleanUp();

    void SetTarget(Entity* entity);
    void SetOffset(const Vector2& offset);

private:
    void Move();

    Entity* target = nullptr;
    Vector2 lastPosition;

    // Camera settings
    float smoothSpeed = 5.0f;
    float recenterSpeed = 3.0f;
    float stationaryThreshold = 1.0f;
    float stationaryTimer = 0.0f;
    float deadZoneHorizontalRadius = 50.0f;
    float deadZoneVerticalRadius = 30.0f;
};