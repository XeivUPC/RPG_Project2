#include "NpcCharacter.h"
#include "Pooling.h"

#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleAssetDataBase.h"
#include "ModuleUpdater.h"
#include "DrawingTools.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"

NpcCharacter::NpcCharacter()
{
    
    texture = Engine::Instance().m_assetsDB->GetTexture("npc_test");

    renderLayer = 3;
    renderOffsetSorting = { 0,2 };
}

NpcCharacter::~NpcCharacter()
{
}

bool NpcCharacter::Update()
{
    SearchPath();
    Move();
    return true;
}

void NpcCharacter::Render()
{
    SDL_Rect rect = { 0,0,64,64 };
    Engine::Instance().m_render->painter().RenderTexture(*texture, position, &rect, { 1.f,1.f }, 0, { 0.5f,0.75f });
}

bool NpcCharacter::CleanUp()
{
    Pooling::Instance().ReturnObject(this);
    return true;
}

void NpcCharacter::SearchPath()
{
}

void NpcCharacter::Move()
{
    position = body->GetPhysicPosition();
}

void NpcCharacter::InitPoolObject()
{
    Engine::Instance().m_render->AddToRenderQueue(*this, *this);
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");

    body = Engine::Instance().m_physics->factory().CreateBox({ 0,0.2f }, 0.5f, 0.2f);
    body->SetType(PhysBody::BodyType::Kinematic);
    body->SetFriction(0, 0);
    body->body->SetFixedRotation(true);
}

void NpcCharacter::ResetPoolObject()
{
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_render->RemoveFomRenderQueue(*this);
    delete body;
}
