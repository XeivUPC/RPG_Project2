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
class UIAnimatedImage;
class UIButton;
class UITextBox;
class UIAnimatedImage;
class UISlider;
class UIToggle;
class Animator;
class AlertDisplayerCG;
struct Mix_Chunk;

class CombatCG : public UICanvas
{
public:
	CombatCG(CombatSystem* _combatSystem);
	~CombatCG();

	void UpdateCanvas();
	void LoadCanvas();
	void UnloadCanvas();
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

		UIImage* powerImg = nullptr;
		UITextBox* power = nullptr;

		UIImage* accuracyImg = nullptr;
		UITextBox* accuracy = nullptr;

		Attack* currentAttack = nullptr;
	};

	struct UIEndScreen {
		UIImage* endBg;
		UITextBox* resultText;
		UIButton* continueBtn;
	};

	struct UICharacterSlot {

		UIButton* characterClick = nullptr;
		UIAnimatedImage* characterImage = nullptr;
		CombatSystem::CharacterReference* characterRef=nullptr;

		UITextBox* lvl = nullptr;
		UITextBox* name = nullptr;

		UIToggle* poison = nullptr;
		UIToggle* burn = nullptr;
		UIToggle* regeneration = nullptr;

		UIImage* hpBar = nullptr;
		int hpBarMaxWidth = 0;

		UIImage* overlay = nullptr;

		UIImage* attackDone = nullptr;

		UIImage* selectedCharacterIndicator = nullptr;

		UIImage* selectedCharacterTarget = nullptr;

		UIAnimatedImage* combatEffect = nullptr;

	};



	/// CreationFunctions
	UIAttackButton CreateUIAttackButton(int attackIndex, Vector2 btn_position);
	void CreateUIAttackInformation();
	UICharacterSlot CreateUICharacterSlot(CombatSystem::CharacterReference* value, Vector2 slot_position);
	void CreateUIExtras();
	void CreateEndScreen();
	void SetEndScreen();


	void ShowAttackInformation(int attackIndex);
	void HideAttackInformation(int attackIndex);
	void HideAttackInformation();

	void SelectCharacter(UICharacterSlot& character);
	void DeselectChatacter();

	void ShowAllPossibleTargets();
	void HideAllPossibleTargets();
	vector<UICharacterSlot*> GetPossibleTargets();
	int GetPossibleTargetsAmount();


	void SelectTarget(UICharacterSlot& character);
	void RemoveAllTargets();

	void SelectAttack(int attackIndex);
	void ConfirmAttack();
	void EndTurn();

	void OnCombatStateChanged();

	void SetTargetSelectionMode(bool mode);

	UICharacterSlot* GetSlotByCharacter(CombatSystem::CharacterReference* reference);
	Vector2Int GetSlotPosition(CombatSystem::CharacterType team, int teamMemberIndex, int teamMembersAmount);

	void FinishAttackVisuals(UIAnimatedImage* characterImage);
	void FinishHurtVisuals(UIAnimatedImage* characterImage, CombatSystem::CharacterReference* ref);
	//void FinishEffectVisuals(UIAnimatedImage* characterImage);

private:
	CombatSystem* combat = nullptr;

	/// UI
	UIImage* combatBg = nullptr;
	UIImage* combatLayout = nullptr;

	UIButton* confirmAttack = nullptr;
	UIButton* cancelAttack = nullptr;
	UIButton* passTurn = nullptr;

	UIAttackInformation attackInfo;
	vector<UIAttackButton> attackButtons;
	vector<UICharacterSlot> charactersSlot;

	UIEndScreen endScreen;

	/// TrackingData
	UICharacterSlot* selectedCharacter = nullptr;
	vector<UICharacterSlot*> targetCharacters;
	UIAttackButton* selectedAttack = nullptr;

	unordered_map<CombatSystem::CharacterReference*, pair<Attack*, vector<CombatSystem::CharacterReference*>>> attacksToExecute;

	/// Condition
	bool selectingTargets = false;


	/// Extra
	AlertDisplayerCG* alert;
	UITextBox* debug_immortalEnabled = nullptr;
	pair<bool, bool> visualEffects = pair<bool,bool>(false,false);	//First bool = attacker  | Second bool = targets
	pair<bool, bool> animationEffect = pair<bool,bool>(false,true);	//First bool = animation | Second bool = effect
	pair<int, int> targetVisualsCompleted = pair<int, int>(0, 0);	//First bool = animation | Second bool = effect
	pair<int, int> targetVisualsRequest = pair<int, int>(0, 0);	//First bool = animation | Second bool = effect
	bool firstTick = true;


	//// Sounds
	Mix_Chunk* combat_hurt = nullptr;
	Mix_Chunk* combat_dead = nullptr;
	Mix_Chunk* combat_magic = nullptr;
	Mix_Chunk* combat_physic = nullptr;
};
