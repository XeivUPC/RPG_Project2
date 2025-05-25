#pragma once
#include <string>

class IDialogable {
public:
	virtual ~IDialogable() {}
	virtual bool UseDialogue() = 0;

public:
	std::string dialogPath = "";
	int state = 0;
protected:
};