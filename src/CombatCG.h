#pragma once
#include "UICanvas.h"
#include "StepTimer.h"
#include "SystemEvent.h"
#include "CombatSystem.h"
#include <string>
#include <vector>
#include <unordered_map>

class CombatSystem;
class UIElement;
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
	void SetUpCanvas();

	CombatSystem* combat;

private:
	struct OverworldCharacter
	{
		CombatSystem::CharacterReference* CharacterId;
		int position;
		Animator* animator = nullptr;
		UIButton* btn = nullptr;
		UITextBox* name = nullptr;
		UISlider* health = nullptr;

		bool attackDone = false;

		OverworldCharacter(CombatSystem::CharacterReference* characterReference, int _position)
		{
			CharacterId = characterReference;
			position = _position;
		}
	};

	struct AttackSelectionButtonData {
		Attack* attack;
		UIButton* btn;
		UITextBox* btn_text;
		EventId onClick;
	};

	CombatSystem::CharacterReference current;

	UIImage* attackSelectionBackground = nullptr;
	//UIImage* currentCharacter = nullptr;
	//UIButton* attackConfirm = nullptr;
	//UITextBox* description = nullptr;
	//vector<AttackSelectionButtonData> availableAttacks;


	void EnableAttackDescription(int attackIndex);
	void DisableAttackDescription(int attackIndex);
	int TeamMembersQuantity(CombatSystem::CharacterType _type);
	
	int currentCombatCharacter = 0;



	///////////////NEW
	vector<OverworldCharacter> charactersInCombat;

	UIImage* combatBackground = nullptr;

	UIImage* mainInfoArea = nullptr;

	UIElement* attackButtonHolder = nullptr;
	vector<AttackSelectionButtonData> attackButtons;
	UIImage* attackButtonsCover = nullptr;

	UITextBox* attackDescription = nullptr;
	int currentAttackDescription = -1;

	UIImage* selectedCharacterIndicator = nullptr;

	UIButton* endTurnButton = nullptr;
	UIButton* confirmAtackButton = nullptr;



	void SelectCharacter(int characterIndex);
	void SelectAttack(int attackIndex);
	void AddAttack();
	OverworldCharacter* selectedCharacter = nullptr;
	AttackSelectionButtonData* selectedAttack = nullptr;
	
	UIButton* returnToSelectCharacterButton = nullptr;
	bool selectingTarget = false;
	void SetSelectingTargetMode(bool mode);


	void SelectTarget(OverworldCharacter& character);
	void DeselectTarget(OverworldCharacter& character);
	int selectionableTargets = 1;

	CombatSystem::CharacterType seleccionableTargetsType = CombatSystem::Enemy;
	vector<OverworldCharacter*> selectedTargets;

	unordered_map<CombatSystem::CharacterReference*, pair<Attack*,vector<CombatSystem::CharacterReference*>>> attacksToExecute;

	void EndTurn();

	void OnStateChanged();
};
