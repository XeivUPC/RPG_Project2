#pragma once
class IInteractuable {
public:
	virtual void Interact() = 0;
	virtual ~IInteractuable() {}
};