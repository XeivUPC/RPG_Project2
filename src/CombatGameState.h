#pragma once
#include "GameState.h"

class CombatGameState : public GameState {
private:
	int turn = 0;
	std::vector<pair<int, bool>> npcActionCompletedList;
public:
	CombatGameState();
	bool PreUpdateState() override;
	bool UpdateState() override;
	bool PostUpdateState() override;
	void StateSelected() override;
	void StateDeselected() override;
};