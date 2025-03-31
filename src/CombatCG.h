#pragma once
#include "UICanvas.h"
#include "StepTimer.h"
#include "SystemEvent.h"
#include "CombatSystem.h"
#include <string>
#include <vector>

class CombatSystem;
class UIImage;
class UIButton;
class UITextBox;
class UISlider;
class Animator;

class CombatCG : public UICanvas
{
public:
	CombatCG(CombatSystem* _combatSystem);
	void UpdateCanvas();
	CombatSystem* combat;

private:
	struct OverworldCharacter
	{
		CombatSystem::CharacterReference CharacterId;
		Animator* animator = nullptr;
		UIButton* btn = nullptr;
		UITextBox* name = nullptr;
		UISlider* health = nullptr;

		OverworldCharacter(int position, CombatSystem::CharacterType type)
		{
			CharacterId.position = position;
			CharacterId.team = type;
		}
	};

	struct AttackSelectionButtonData {
		Attack* attack;
		UIButton* btn;
		UITextBox* btn_text;
		EventId onClick;
	};

	CombatSystem::CharacterReference current;

	UIImage* combatBackground = nullptr;
	UIImage* attackSelectionBackground = nullptr;
	UIImage* currentCharacter = nullptr;
	UIButton* attackConfirm = nullptr;
	UITextBox* description = nullptr;
	vector<AttackSelectionButtonData> availableAttacks;
	vector<OverworldCharacter> availableTargets;

	void ModifyAttack();
	void ModifyAttackTarget();
	void ConfirmAttack();

	void EnableAttackDescription(int attackIndex);
	void DisableAttackDescription(int attackIndex);
	void SwitchCharacter();
	int TeamMembersQuantity(CombatSystem::CharacterType _type);
	int currentAttackDescription = -1;
	int currentCombatCharacter = 0;
};
