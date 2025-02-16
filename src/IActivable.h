#pragma once
class IActivable {
public:
	virtual ~IActivable() {}

	virtual void Activate() = 0;
	virtual void Desactivate() = 0;
	virtual bool IsActive() = 0;
protected:
	bool isActive = true;
};