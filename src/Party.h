
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

	bool AddFollower(int id);
	bool RemoveFollower(int id);
	bool SetLeader(int id);

	bool EditMember(int index, int id);
	bool SwapMembers(int id, int id2);
	void ClearParty();

	void SetMaxPartySize(int max);
	int GetMaxPartySize() const;

	int GetPartySize() const;
	int GetFollowersAmount() const;

	CharacterDatabase::CharacterData* GetLeader() const;
	vector<CharacterDatabase::CharacterData*> GetParty() const;
	vector<CharacterDatabase::CharacterData*> GetFollowers() const;

	int GetLeaderId() const;
	vector<int> GetActivePartyIds();
	vector<int> GetActiveFollowersIds() const;

	CharacterDatabase::CharacterData* GetMember(int index) const;

public:
	SystemEvent<> onPartyChanged;

private:
	bool IsMemberInParty(int id) const;

private:
	CharacterDatabase::CharacterData* leader = nullptr;
	vector<CharacterDatabase::CharacterData*> followers;
	int followersMaxAmount = 3;



};