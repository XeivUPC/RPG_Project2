#include "CombatGameState.h"
#include "Engine.h"
#include "GameScene.h"

CombatGameState::CombatGameState()
{
	Engine::Instance().s_game->combatCanvas->isVisible = false;
}
