#pragma once
class IUpdateable {
public:
	virtual bool PreUpdate() = 0;
	virtual bool Update() = 0;
	virtual bool PostUpdate() = 0;
	virtual ~IUpdateable() {}

	bool isPaused = false;

	int priority_updating = 0;
};