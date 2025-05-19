#pragma once
#include "GameState.h"
#include <vector>

class CombatGameState : public GameState {
public:
	CombatGameState();
	bool PreUpdateState() override;
	bool UpdateState() override;
	bool PostUpdateState() override;
	void StateSelected() override;
	void StateDeselected() override;

public:

private:
	void OnLoadingEnd();
	int lastState = 0;
private:

};