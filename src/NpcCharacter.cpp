#include "NpcCharacter.h"
#include "Pooling.h"

#include "Engine.h"
#include "ModuleRender.h"
#include "GameScene.h"
#include "ModuleAssetDataBase.h"
#include "ModuleUpdater.h"
#include "ModuleInput.h"
#include "DrawingTools.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"
#include "Animator.h"
#include "AnimationClip.h"

NpcCharacter::NpcCharacter()
{
    texture = Engine::Instance().m_assetsDB->GetTexture("pj_test");

    renderLayer = 3;
    renderOffsetSorting = { 0,2 };
    animator = new Animator
    (
        { 
            AnimationClip("test1", true, false, 0.5f,
            {
                Sprite(texture, {58,49,31,49}),
                Sprite(texture, {107,52,31,46})
            },&position,&scale),
            AnimationClip("test2", true, false, 0.5f,
            {
                Sprite(texture, {58,99,31,49}),
                Sprite(texture, {107,101,31,46})
            },&position,&scale)
        }, 0
    );
}

NpcCharacter::~NpcCharacter()
{
}

bool NpcCharacter::Update()
{

    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
        animator->Animate("test1");
    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
        animator->Animate("test2");

    if(!Engine::Instance().m_render->IsRectCameraVisible(animator->clip()->GetAnimationSpace()))
        isVisible = false;
    else
        isVisible = true;

    animator->clip()->UpdateClip();
    SearchPath();
    Move();
    return true;
}

void NpcCharacter::Render()
{
    
    animator->clip()->RenderClip();
}

bool NpcCharacter::CleanUp()
{
    animator->CleanUp();
    delete animator;
    Pooling::Instance().ReturnObject(this);
    return true;
}

void NpcCharacter::SetNpcData(int _npcId, Vector2 _position)
{
    npcId = _npcId;
    SetPosition(position);
}

void NpcCharacter::SearchPath()
{
}

void NpcCharacter::Move()
{
    position = body->GetPhysicPosition();
}

void NpcCharacter::Interact()
{
    printf("Ey\n");
    Engine::Instance().s_game->SetState(GameScene::State::Dialogue);
    Engine::Instance().s_game->SetDialogue("Assets/Dialogues/test2.json");
}

void NpcCharacter::InitPoolObject()
{
    Engine::Instance().m_render->AddToRenderQueue(*this, *this);
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_updater->AddToUpdateGroup(*this, "Entity");

    body = Engine::Instance().m_physics->factory().CreateBox({ 0,0.2f }, 0.5f, 0.2f);
    body->SetType(PhysBody::BodyType::Kinematic);
    body->SetFriction(0, 0);
    body->SetFixedRotation(true);

    body->data = (uintptr_t)((IInteractuable*)this);

    int fixtureIndex = Engine::Instance().m_physics->factory().AddCircle(body, { 0,0.1f }, 1.0f);
    body->SetSensor(fixtureIndex, true);
    ModulePhysics::Layer category, mask;
    category.flags.interactable_layer = 1;
    mask.flags.interactable_layer = 1;
    body->SetFilter(fixtureIndex, category.rawValue, mask.rawValue, 0);


   
}

void NpcCharacter::ResetPoolObject()
{
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
    Engine::Instance().m_render->RemoveFomRenderQueue(*this);
    delete body;
}
