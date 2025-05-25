#pragma once
#include "SystemEvent.h"
#include "Entity.h"
#include <string>
#include <unordered_map>

using namespace std;

class PuzzleElement : public Entity {
public:
	PuzzleElement();
	~PuzzleElement();

	string GetId() const;

	virtual void RecieveCall(string _id, unordered_map<string, string> _params) = 0;
	virtual void SendCall() = 0;

	virtual void Complete() = 0;

public:

private:


private:
	
protected:

protected:
	string id;
	EventId eventId;
};