#include "DialogueGameState.h"
#include "Engine.h"
#include "GameScene.h"
#include "UIDialogueBoxCG.h"

bool DialogueGameState::PreUpdateState()
{
    return true;
}

bool DialogueGameState::UpdateState()
{
    Engine::Instance().s_game->dialogueCanvas->UpdateCanvas();

    return true;
}

bool DialogueGameState::PostUpdateState()
{
    return true;
}

void DialogueGameState::StateSelected()
{
}

void DialogueGameState::StateDeselected()
{
}
