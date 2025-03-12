#include "PauseGameState.h"
#include "Engine.h"
#include "GameScene.h"
#include "PauseMenuCG.h"

bool PauseGameState::PreUpdateState()
{
    return true;
}

bool PauseGameState::UpdateState()
{
    Engine::Instance().s_game->pauseCanvas->UpdateCanvas();
    return true;
}

bool PauseGameState::PostUpdateState()
{
    return true;
}

void PauseGameState::StateSelected()
{
    Engine::Instance().s_game->pauseCanvas->isVisible = true;
}

void PauseGameState::StateDeselected()
{
    if (Engine::Instance().s_game->pauseCanvas->IsInteractable()) {
        Engine::Instance().s_game->pauseCanvas->SetInteractable(false);
        Engine::Instance().s_game->pauseCanvas->UpdateCanvas();
        Engine::Instance().s_game->pauseCanvas->isVisible = false;
        Engine::Instance().s_game->pauseCanvas->SetInteractable(true);
    }
}
