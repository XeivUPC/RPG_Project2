#include "ExploringGameState.h"
#include "Engine.h"
#include "ModuleInput.h"
#include "ModuleUpdater.h"
#include "ModuleCursor.h"
#include "GameplayCG.h"
#include "GameScene.h"

bool ExploringGameState::PreUpdateState()
{
    return true;
}

bool ExploringGameState::UpdateState()
{
    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_P))
        Engine::Instance().s_game->SetState(GameScene::State::Menu);

    Engine::Instance().s_game->gameplayCanvas->UpdateCanvas();
    return true;
}

bool ExploringGameState::PostUpdateState()
{
    return true;
}

void ExploringGameState::StateSelected()
{
    Engine::Instance().m_updater->ResumeUpdateGroup("Entity");
    Engine::Instance().m_cursor->HideAllCursors();
}

void ExploringGameState::StateDeselected()
{
}
