#pragma once
#include "GameState.h"

class PauseGameState : public GameState {
public:
	bool PreUpdateState() override;
	bool UpdateState() override;
	bool PostUpdateState() override;
	void StateSelected() override;
	void StateDeselected() override;
};