#include "PauseGameState.h"
#include "Engine.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "PauseMenuCG.h"
#include "FadeCG.h"

bool PauseGameState::PreUpdateState()
{
    return true;
}

bool PauseGameState::UpdateState()
{
    GameScene* scene = Engine::Instance().s_game;
    scene->pauseCanvas->UpdateCanvas();
    
    return true;
}

bool PauseGameState::PostUpdateState()
{
    GameScene* scene = Engine::Instance().s_game;
    if (scene->exitGame && !scene->fade->IsFading()) {
        Engine::Instance().s_title->Activate();
        scene->Desactivate();
    }
    else if (scene->exitGame) {
        if (Engine::Instance().s_game->pauseCanvas->IsInteractable()) {
            Engine::Instance().s_game->pauseCanvas->SetInteractable(false);
        }
    }
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
