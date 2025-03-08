#pragma once

class GameState {
public:
	~GameState() {}
	virtual bool PreUpdateState() = 0;
	virtual bool UpdateState() = 0;
	virtual bool PostUpdateState() = 0;
	virtual void RenderState() = 0;
	virtual void StateSelected() = 0;
	virtual void StateDeselected() = 0;
};