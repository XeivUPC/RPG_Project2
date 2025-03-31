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
		Animator* animator;
		UIButton* btn;
		UITextBox* name;
		UISlider* health;
	};

	struct AttackSelectionButtonData {
		Attack* attack;
		UIButton* btn;
		UITextBox* btn_text;
		EventId onClick;
	};

	UIImage* combatBackground = nullptr;
	UIImage* attackSelectionBackground = nullptr;
	UIButton* attackConfirm = nullptr;
	UITextBox* description = nullptr;
	vector<AttackSelectionButtonData> availableAttacks;
	vector<OverworldCharacter> availableTargets;

	void EnableAttackDescription(int attackIndex);
	void DisableAttackDescription(int attackIndex);
	int currentAttackDescription = -1;
};
