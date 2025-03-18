#include "DialogueGameState.h"
#include "Engine.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleUpdater.h"
#include "GameScene.h"
#include "UIDialogueBoxCG.h"
#include "DialogueSystem.h"

bool DialogueGameState::PreUpdateState()
{
    return true;
}

bool DialogueGameState::UpdateState()
{
    UIDialogueBoxCG* uiDialogue = Engine::Instance().s_game->dialogueCanvas;
    uiDialogue->UpdateCanvas();
    if (!uiDialogue->dialogue->IsDialogueActive()) {
        Engine::Instance().s_game->dialogueCanvas->isVisible = false;
        Engine::Instance().s_game->SetState(GameScene::State::Exploring);
    }
    else {
        Engine::Instance().s_game->dialogueCanvas->isVisible = true;
    }

    if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_P))
        Engine::Instance().s_game->SetState(GameScene::State::Menu);
    return true;
}

bool DialogueGameState::PostUpdateState()
{
    return true;
}

void DialogueGameState::StateSelected()
{
    Engine::Instance().m_updater->PauseUpdateGroup("Entity");
    Engine::Instance().m_physics->PauseSimulation();
    Engine::Instance().s_game->dialogueCanvas->SetInteractable(true);
    Engine::Instance().s_game->dialogueCanvas->isVisible = true;
}

void DialogueGameState::StateDeselected()
{
    if (Engine::Instance().m_physics->IsSimulationPaused()) {
        Engine::Instance().m_physics->StartSimulation();
    }
    Engine::Instance().s_game->dialogueCanvas->SetInteractable(false);
    Engine::Instance().s_game->dialogueCanvas->isVisible = false;
    Engine::Instance().s_game->dialogueCanvas->UpdateCanvas();
}
