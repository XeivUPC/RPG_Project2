
#pragma once
#include "CharacterDatabase.h"
#include "SystemEvent.h"

#include <string>
#include <vector>

using namespace std;

class Party {
public:
	Party(int defaultId);
	~Party();

	bool AddMember(int id);
	bool RemoveMember(int id);
	bool SetLeader(int index);

	bool EditMember(int index, int id);
	bool SwapMembers(int id, int id2);
	void ClearParty();

	void SetMaxMembers(int max);
	int GetMaxMembers() const;
	int GetCurrentMembers() const;

	CharacterDatabase::CharacterData* GetLeader() const;
	vector<CharacterDatabase::CharacterData*> GetMembers() const;
	CharacterDatabase::CharacterData* GetMember(int index) const;

public:
	SystemEvent<> onPartyChanged;

private:
	bool IsMemberInParty(int id) const;

private:
	CharacterDatabase::CharacterData* leader = nullptr;
	vector<CharacterDatabase::CharacterData*> members;
	int maxMembers = 3;
};