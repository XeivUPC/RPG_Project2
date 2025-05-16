#include "CombatGameState.h"
#include "CombatCG.h"
#include "FadeCG.h"
#include "CombatSystem.h"
#include "Engine.h"
#include "GameScene.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleAssetDatabase.h"
#include "ModuleAudio.h"
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

	else if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_P))
		Engine::Instance().s_game->SetState(GameScene::State::Menu);
	return true;
}

bool CombatGameState::PostUpdateState()
{
	
	return true;
}

void CombatGameState::StateSelected()
{
	Engine::Instance().s_game->PauseRain();

	Engine::Instance().m_audio->PlayMusicAsync(Engine::Instance().m_assetsDB->GetMusic("zaliumAnthemOrchestra"), 100);

	Engine::Instance().m_updater->PauseUpdateGroup("Entity");
	Engine::Instance().m_physics->PauseSimulation();
	Engine::Instance().s_game->combatCanvas->SetInteractable(true);
	Engine::Instance().s_game->combatCanvas->isVisible = true;
	Engine::Instance().m_cursor->ShowCustomCursor();

	if (Engine::Instance().s_game->previous_state != GameScene::State::Menu) {
		Engine::Instance().s_game->fade->onFadeEnd.Subscribe([this]() {OnLoadingEnd();});
		Engine::Instance().s_game->fade->FadeTo(0.5f, 255);
	}

}

void CombatGameState::StateDeselected()
{
	Engine::Instance().s_game->ResumeRain();
	if (Engine::Instance().m_physics->IsSimulationPaused()) {
		Engine::Instance().m_physics->StartSimulation();
	}
	Engine::Instance().s_game->combatCanvas->SetInteractable(false);

	if (Engine::Instance().s_game->state != GameScene::State::Menu)
		Engine::Instance().s_game->combatCanvas->UnloadCanvas();
}

void CombatGameState::OnLoadingEnd()
{
	if (Engine::Instance().s_game->previous_state != GameScene::State::Menu) {
		Engine::Instance().s_game->fade->FadeTo(0.5f, 0);
		Engine::Instance().s_game->combatSystem->StartCombat();
		Engine::Instance().s_game->combatCanvas->LoadCanvas();
	}
	
}
