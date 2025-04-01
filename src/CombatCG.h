#pragma once
#include "UICanvas.h"
#include "StepTimer.h"
#include "SystemEvent.h"
#include "CombatSystem.h"

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
		UIButton* character = nullptr;
		CombatSystem::CharacterReference* characterRef=nullptr;

		UITextBox* lvl = nullptr;
		UITextBox* name = nullptr;

		UIToggle* poison = nullptr;
		UIToggle* burn = nullptr;
		UIToggle* regeneration = nullptr;

		UIImage* hpBar = nullptr;
		UIImage* overlay = nullptr;

		UIImage* selectedCharacterIndicator = nullptr;
		UIImage* selectedCharacterTarget = nullptr;
	};

	/// CreationFunctions
	UIAttackButton CreateUIAttackButton(Attack* value, Vector2 btn_position);
	void CreateUIAttackInformation();
	UICharacterSlot CreateUICharacterSlot(CombatSystem::CharacterReference* value, Vector2 slot_position);


	void ShowAttackInformation(Attack* attackIndex);
	void HideAttackInformation(Attack* attackIndex);

private:
	CombatSystem* combat = nullptr;

	/// UI
	UIImage* combatBg = nullptr;
	UIImage* combatLayout = nullptr;
	UIAttackInformation attackInfo;
	vector<UIAttackButton> attackButtons;
	vector<UICharacterSlot> charactersSlot;

	/// TrackingData
};
