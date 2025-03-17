#include "CameraController.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleTime.h"
#include "ModuleUpdater.h"

CameraController::CameraController()
{
	Engine::Instance().m_render->AddToRenderQueue(*this, *this);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);
	Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");
}

CameraController::~CameraController()
{
}

bool CameraController::PostUpdate()
{
	Move();
	return true;
}

bool CameraController::CleanUp()
{
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

void CameraController::SetOffset(Vector2 offset)
{
	Engine::Instance().m_render->SetCameraOffset(offset);
}

void CameraController::Move()
{
    if (!target) return;

    Vector2 targetPos = target->GetPosition();
    Vector2 delta = targetPos - position;

    bool moveX = abs(delta.x) > deadZoneHorizontalRadius;
    bool moveY = abs(delta.y) > deadZoneVerticalRadius;

    if (moveX || moveY) {
        Vector2 desiredPos = position;

        if (moveX) {
            float direction = delta.x > 0 ? 1.0f : -1.0f;
            desiredPos.x = targetPos.x - direction * deadZoneHorizontalRadius;
        }
        if (moveY) {
            float direction = delta.y > 0 ? 1.0f : -1.0f;
            desiredPos.y = targetPos.y - direction * deadZoneVerticalRadius;
        }

        float deltaTime = ModuleTime::deltaTime;
        position = Vector2::Lerp(position, desiredPos, smoothSpeed * deltaTime);

        Engine::Instance().m_render->SetCameraPosition(position);
    }
}
