
#pragma once
#include "CharacterDatabase.h"
#include "SystemEvent.h"

#include <string>
#include <vector>

using namespace std;

class Party {
public:
	Party(string defaultId);
	~Party();

	bool AddPartyMemeber(string id);
	bool AddMemeber(string id);
	bool RemovePartyMemeber(string id);
	bool RemoveMemeber(string id);


	bool EditPartyMember(int index, string id);
	bool SwapPartyMembers(string id, string id2);
	void ClearParty();
	void ClearMemebers();

	void SetMaxPartySize(int max);
	int GetMaxPartySize() const;

	int GetPartySize() const;
	int GetMemebersAmount() const;

	bool IsMemberInParty(string id) const;
	bool IsMemberUnlocked(string id) const;

	CharacterDatabase::CharacterDefinition* GetPartyLeader() const;
	vector<CharacterDatabase::CharacterDefinition*> GetParty(bool removeLeader = false) const;
	vector<CharacterDatabase::CharacterDefinition*> GetMemebers() const;

	string GetPartyLeaderId() const;
	vector<string> GetPartyIds(bool removeLeader = false) const;
	vector<string> GetMembersIds() const;
	CharacterDatabase::CharacterDefinition* GetCharacterFromParty(int index) const;
	CharacterDatabase::CharacterDefinition* GetCharacterFromMembers(string id) const;

public:
	SystemEvent<> onPartyChanged;
	SystemEvent<> onMembersChanged;

private:
	bool IsPartyLeader(string id) const;
	void SortMemebers();
	bool SetPartyLeader();

private:

	CharacterDatabase::CharacterDefinition* leader = nullptr;
	vector<CharacterDatabase::CharacterDefinition*> party;
	int partyMaxSize = 4;

	vector<CharacterDatabase::CharacterDefinition*> members;
};