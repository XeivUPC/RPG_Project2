#include "CombatGameState.h"
#include "CombatCG.h"
#include "CombatSystem.h"
#include "Engine.h"
#include "GameScene.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleCursor.h"
#include "ModuleUpdater.h"

CombatGameState::CombatGameState()
{
	Engine::Instance().s_game->combatCanvas->isVisible = false;
}

bool CombatGameState::PreUpdateState()
{
	return true;
}

bool CombatGameState::UpdateState()
{
	CombatCG* combatCanvas = Engine::Instance().s_game->combatCanvas;
	combatCanvas->UpdateCanvas();
	return true;
}

bool CombatGameState::PostUpdateState()
{
	return true;
}

void CombatGameState::StateSelected()
{
	Engine::Instance().m_updater->PauseUpdateGroup("Entity");
	Engine::Instance().m_physics->PauseSimulation();
	Engine::Instance().s_game->combatCanvas->SetInteractable(true);
	Engine::Instance().s_game->combatCanvas->isVisible = true;
	Engine::Instance().m_cursor->ShowCustomCursor();


	Engine::Instance().s_game->combatSystem->AddPartyToCombat(vector<int>{1,0,1,1}, CombatSystem::Ally);
	Engine::Instance().s_game->combatSystem->AddPartyToCombat(vector<int>{1, 0, 1}, CombatSystem::Enemy);


	Engine::Instance().s_game->combatSystem->StartCombat();
	Engine::Instance().s_game->combatCanvas->LoadCanvas();
}

void CombatGameState::StateDeselected()
{
	if (Engine::Instance().m_physics->IsSimulationPaused()) {
		Engine::Instance().m_physics->StartSimulation();
	}
	Engine::Instance().s_game->combatCanvas->SetInteractable(false);
	Engine::Instance().s_game->combatCanvas->UpdateCanvas();
}
