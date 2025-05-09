#include "CameraController.h"
#include "Entity.h"
#include "Engine.h"
#include "ModuleUpdater.h"
#include "ModuleRender.h"
#include "ModuleTime.h"

#include <algorithm> 

CameraController::CameraController() {
}

CameraController::~CameraController() = default;



bool CameraController::UpdateCamera()
{
    Move();
    return true;
}

bool CameraController::CleanUp() {
    return true;
}

void CameraController::SetTarget(Entity* entity) {
    target = entity;
    if (target) {
        position = target->GetPosition();

        Engine::Instance().m_render->SetCameraPosition(position);
    }
}

void CameraController::SetOffset(const Vector2& offset) {
    Engine::Instance().m_render->SetCameraOffset(offset);
}

void CameraController::SetBounds(const Vector2& position, const Vector2& size)
{
    bounds = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
}

void CameraController::Move() {
    if (!target) return;

    
    const float alpha = Engine::Instance().m_time->GetPhysicsInterpolationAlpha();

    lastPosition = position;

    const Vector2 currentTargetPos = target->GetPosition();
    const Vector2 interpolatedTargetPos = Vector2::Lerp(lastPosition, currentTargetPos, alpha);

    position = interpolatedTargetPos;

    const Vector2 roundedPosition = {
        round(position.x * 100.0f) / 100.0f,
        round(position.y * 100.0f) / 100.0f
    };

    position = roundedPosition;

    Camera camera = Engine::Instance().m_render->GetCamera();
    SDL_Rect cameraRect = camera.GetRect();

    const float minX = bounds.x - camera.offset.x / camera.zoom;
    const float minY = bounds.y - camera.offset.y / camera.zoom;
    const float maxX = bounds.x + bounds.w - cameraRect.w - camera.offset.x / camera.zoom;
    const float maxY = bounds.y + bounds.h - cameraRect.h - camera.offset.y / camera.zoom;

    if (minX>maxX) {
        position.x = maxX;
    }else
        position.x = std::clamp(position.x, minX, maxX);
    if (minY > maxY) {
        position.y = maxY;
    }
    else
        position.y = std::clamp(position.y, minY, maxY);

    Engine::Instance().m_render->SetCameraPosition(position);
}