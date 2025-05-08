#include "ExploringGameState.h"
#include "Engine.h"
#include "ModuleInput.h"
#include "ModuleTime.h"
#include "ModuleUpdater.h"
#include "ModuleCursor.h"
#include "GameplayCG.h"
#include "ScreenEffectsCG.h"
#include "GameScene.h"

bool ExploringGameState::PreUpdateState()
{
    return true;
}

bool ExploringGameState::UpdateState()
{
    GameScene* game = Engine::Instance().s_game;


    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
        game->SetState(GameScene::State::Menu);


    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
        game->screenEffectsCanvas->SwitchRain();

    game->clock.Step(ModuleTime::deltaTime * game->timeScale);
    int clockTime = game->clock.ReadSec();
    if (clockTime >= 86400.0)
        game->clock.Start();

    game->gameplayCanvas->UpdateCanvas();
    game->screenEffectsCanvas->UpdateCanvas();
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
