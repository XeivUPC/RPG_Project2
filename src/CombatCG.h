#pragma once
#include "UICanvas.h"
#include "StepTimer.h"
#include "SystemEvent.h"
#include "CharacterDatabase.h"

#include <string>
#include <vector>
#include <unordered_map>

class CombatSystem;
class Attack;
class UIElement;
class UIImage;
class UIButton;
class UITextBox;
class UISlider;
class UIToggle;
class Animator;

class CombatCG : public UICanvas
{
public:
	CombatCG(CombatSystem* _combatSystem);
	void UpdateCanvas();
	void LoadCanvas();
public:

private:
	struct UIAttackButton {
		UIButton* btn = nullptr;
		UITextBox* text = nullptr;
		Attack* attack = nullptr;
	};

	struct UIAttackInformation {
		UITextBox* name = nullptr;
		UITextBox* description = nullptr;
		UITextBox* power = nullptr;
		UITextBox* accuracy = nullptr;

		Attack* currentAttack = nullptr;
	};

	struct UICharacterSlot {
		UITextBox* lvl = nullptr;
		UITextBox* name = nullptr;

		UIToggle* posion = nullptr;
		UIToggle* burn = nullptr;
		UIToggle* regeneration = nullptr;

		UIImage* hpBar = nullptr;
		UIImage* overlay = nullptr;
	};

	/// CreationFunctions
	UIAttackButton CreateUIAttackButton(Attack* value, Vector2 btn_position);
	void CreateUIAttackInformation();
	UICharacterSlot CreateUICharacterSlot(Attack* value, Vector2 btn_position);


	void ShowAttackInformation(Attack* attackIndex);
	void HideAttackInformation(Attack* attackIndex);

private:
	CombatSystem* combat = nullptr;

	/// UI
	UIImage* combatBg = nullptr;
	UIImage* combatLayout = nullptr;
	UIAttackInformation attackInfo;
	vector<UIAttackButton> attackButtons;

	/// TrackingData
};
