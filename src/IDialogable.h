#pragma once
#include <string>

class IDialogable {
public:
	virtual ~IDialogable() {}
	virtual void UseDialogue() = 0;

public:
	std::string dialogPath = "";
	int state = 0;
protected:
};