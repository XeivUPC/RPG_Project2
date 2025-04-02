#include "CombatSystem.h"

class Attack
{
private:
	

public:

	struct StageModifier {
		string type;

		int value = 0;
		int probability = 0;
		CombatSystem::CharacterType objective;
	};

	struct StatusModifier {
		string type;

		int value = 0;
		int turns = 0;
		int probability = 0;
		CombatSystem::CharacterType objective;
	};

	int id = -1;

	CombatSystem::CharacterType targetType = CombatSystem::Enemy;
	int targetAmount = 0;

	string name = "Name";
	string description = "Description";

	/// Properties
	int priority = 0;
	int accuracy = 100;

	/// Basic Stats
	int power = 0;
	int critRate = 10;

	/// Stages
	unordered_map<string, StageModifier> statsModification;

	/// Effects
	unordered_map<string, StatusModifier> statusEffects;

	/// Extra
	int lifeDewMode = 0;
	int lifeDewAmount = 0;
	int lifeDewEffectiveness=0;

	bool blocksTurn = false;
	CombatSystem::CharacterType blockTurnTarget = CombatSystem::Enemy;


	virtual void DoAttack(CombatSystem::CharacterReference& attacker, std::vector<CombatSystem::CharacterReference*>& target);
};