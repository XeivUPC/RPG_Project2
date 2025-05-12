
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

	bool AddPartyMemeber(int id);
	bool AddMemeber(int id);
	bool RemovePartyMemeber(int id);
	bool RemoveMemeber(int id);


	bool EditPartyMember(int index, int id);
	bool SwapPartyMembers(int id, int id2);
	void ClearParty();
	void ClearMemebers();

	void SetMaxPartySize(int max);
	int GetMaxPartySize() const;

	int GetPartySize() const;
	int GetMemebersAmount() const;

	bool IsMemberInParty(int id) const;
	bool IsMemberUnlocked(int id) const;

	CharacterDatabase::CharacterData* GetPartyLeader() const;
	vector<CharacterDatabase::CharacterData*> GetParty(bool removeLeader = false) const;
	vector<CharacterDatabase::CharacterData*> GetMemebers() const;

	int GetPartyLeaderId() const;
	vector<int> GetPartyIds(bool removeLeader = false) const;
	vector<int> GetMembersIds() const;
	CharacterDatabase::CharacterData* GetCharacterFromParty(int index) const;
	CharacterDatabase::CharacterData* GetCharacterFromMembers(int id) const;

public:
	SystemEvent<> onPartyChanged;
	SystemEvent<> onMembersChanged;

private:
	bool IsPartyLeader(int id) const;
	void SortMemebers();
	bool SetPartyLeader();

private:

	CharacterDatabase::CharacterData* leader = nullptr;
	vector<CharacterDatabase::CharacterData*> party;
	int partyMaxSize = 4;

	vector<CharacterDatabase::CharacterData*> members;
};