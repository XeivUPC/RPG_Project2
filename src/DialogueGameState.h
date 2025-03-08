#pragma once
#include "GameState.h"

class DialogueGameState : public GameState {
public:
	bool PreUpdateState() override;
	bool UpdateState() override;
	bool PostUpdateState() override;
	void RenderState() override;
	void StateSelected() override;
	void StateDeselected() override;
};