#include "CameraController.h"
#include "Entity.h"
#include "Engine.h"
#include "ModuleUpdater.h"
#include "ModuleRender.h"
#include "ModuleTime.h"

CameraController::CameraController() {
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);
    Engine::Instance().m_render->AddToRenderQueue(*this, *this);
}

CameraController::~CameraController() = default;

bool CameraController::PostUpdate() {
    Move();
    return true;
}

bool CameraController::CleanUp() {
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);
    Engine::Instance().m_render->RemoveFomRenderQueue(*this);
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

void CameraController::Move() {
    if (!target) return;

    
    const float alpha = Engine::Instance().m_time->GetPhysicsInterpolationAlpha();

    lastPosition = position;

    const Vector2 currentTargetPos = target->GetPosition();
    const Vector2 interpolatedTargetPos = Vector2::Lerp(lastPosition, currentTargetPos, alpha);

    position = interpolatedTargetPos;

    const Vector2 roundedPosition = {
        std::round(position.x * 100.0f) / 100.0f,
        std::round(position.y * 100.0f) / 100.0f
    };

    position = roundedPosition;

    Engine::Instance().m_render->SetCameraPosition(position);
}