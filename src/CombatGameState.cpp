#include "CombatGameState.h"
#include "CombatCG.h"
#include "FadeCG.h"
#include "CombatSystem.h"
#include "Engine.h"
#include "GameScene.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleCursor.h"
#include "ModuleUpdater.h"
#include "PlayerCharacter.h"
#include "Party.h"

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
	CombatSystem* combatSystem = Engine::Instance().s_game->combatSystem;
	combatCanvas->UpdateCanvas();
	combatSystem->UpdateCombat();

	if (combatSystem->GetCombatState() == CombatSystem::VICTORY || combatSystem->GetCombatState() == CombatSystem::DEFEAT) {
		if (!Engine::Instance().s_game->fade->IsFading()) {

			Engine::Instance().s_game->fade->FadeTo(0.5f, 255);
			Engine::Instance().s_game->fade->onFadeEnd.Subscribe([this]() {Engine::Instance().s_game->fade->FadeTo(0.5f, 0); Engine::Instance().s_game->SetState(GameScene::State::Exploring); });
		}
	}
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


	Engine::Instance().s_game->combatSystem->AddPartyToCombat(Engine::Instance().s_game->GetPlayer()->party->GetPartyIds(), CombatSystem::Ally);
	Engine::Instance().s_game->combatSystem->AddPartyToCombat(vector<int>{5,6, 2}, CombatSystem::Enemy);

	Engine::Instance().s_game->fade->onFadeEnd.Subscribe([this]() {OnLoadingEnd();});

	Engine::Instance().s_game->fade->FadeTo(0.5f, 255);
}

void CombatGameState::StateDeselected()
{
	if (Engine::Instance().m_physics->IsSimulationPaused()) {
		Engine::Instance().m_physics->StartSimulation();
	}
	Engine::Instance().s_game->combatCanvas->SetInteractable(false);
	Engine::Instance().s_game->combatCanvas->UnloadCanvas();
}

void CombatGameState::OnLoadingEnd()
{
	Engine::Instance().s_game->fade->FadeTo(0.5f, 0);
	Engine::Instance().s_game->combatSystem->StartCombat();
	Engine::Instance().s_game->combatCanvas->LoadCanvas();
}
