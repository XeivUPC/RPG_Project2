#pragma once
class IInteractuable {
public:
	virtual void Interact(Vector2 from) = 0;
	virtual ~IInteractuable() {}
};