#pragma once
#include "UICanvas.h"
#include "StepTimer.h"
#include "SystemEvent.h"
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

private:
	CombatSystem* combatSystem;
	struct OverworldCharacter
	{
		int CharacterId;
		Animator* animator;
		UITextBox* name;
		UISlider* health;
	};

	struct AttackSelectionButtonData {
		int AttackId;
		UIButton* btn;
		UITextBox* btn_text;
		EventId onClick;
	};
};
