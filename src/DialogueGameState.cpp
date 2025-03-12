#include "DialogueGameState.h"
#include "Engine.h"
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
    if (!uiDialogue->dialogue->IsDialogueActive())
        Engine::Instance().s_game->SetState(GameScene::State::Menu);
    return true;
}

bool DialogueGameState::PostUpdateState()
{
    return true;
}

void DialogueGameState::StateSelected()
{
    Engine::Instance().s_game->dialogueCanvas->isVisible = true;
}

void DialogueGameState::StateDeselected()
{
    Engine::Instance().s_game->dialogueCanvas->SetInteractable(false);
    Engine::Instance().s_game->dialogueCanvas->UpdateCanvas();
    Engine::Instance().s_game->dialogueCanvas->isVisible = false;
    Engine::Instance().s_game->dialogueCanvas->SetInteractable(true);
}
