#include "ExploringGameState.h"
#include "Engine.h"
#include "ModuleInput.h"
#include "GameScene.h"

bool ExploringGameState::PreUpdateState()
{
    return true;
}

bool ExploringGameState::UpdateState()
{
    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_P))
        Engine::Instance().s_game->SetState(GameScene::State::Menu);
    return true;
}

bool ExploringGameState::PostUpdateState()
{
    return true;
}

void ExploringGameState::StateSelected()
{
}

void ExploringGameState::StateDeselected()
{
}
