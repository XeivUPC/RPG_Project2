#pragma once
#include "Entity.h"

using namespace std;

class CameraController : public Entity {
public:
    CameraController();
    ~CameraController();

    bool PostUpdate();
    bool CleanUp();

    void SetTarget(Entity* entity);
    void SetOffset(const Vector2& offset);
    void SetBounds(const Vector2& position, const Vector2& size);

private:
    void Move();

    Entity* target = nullptr;
    SDL_Rect bounds = {-INT16_MAX,-INT16_MAX ,INT16_MAX ,INT16_MAX};
    Vector2 lastPosition;

    // Camera settings
    float smoothSpeed = 5.0f;
    float recenterSpeed = 3.0f;
    float stationaryThreshold = 1.0f;
    float stationaryTimer = 0.0f;
    float deadZoneHorizontalRadius = 50.0f;
    float deadZoneVerticalRadius = 30.0f;
};