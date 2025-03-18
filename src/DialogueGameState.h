#pragma once
#include "GameState.h"

class DialogueGameState : public GameState {
public:
	DialogueGameState();
	bool PreUpdateState() override;
	bool UpdateState() override;
	bool PostUpdateState() override;
	void StateSelected() override;
	void StateDeselected() override;
};