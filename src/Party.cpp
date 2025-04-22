#include "Party.h"

#include <algorithm>

Party::Party(int defaultId)
{
	AddMemeber(defaultId);
	AddPartyMemeber(defaultId);
	SetPartyLeader(defaultId);
}

Party::~Party()
{
	party.clear();
	leader = nullptr;
}

bool Party::AddPartyMemeber(int id)
{
	if (!IsMemberUnlocked(id))
		return false;
	if (IsMemberInParty(id))
		return false;

	if (party.size() >= partyMaxSize)
		return false;

	CharacterDatabase::CharacterData& member = CharacterDatabase::Instance().GetCharacterData(id);

	party.emplace_back(&member);

	onPartyChanged.Trigger();
    return true;
}

bool Party::AddMemeber(int id)
{
	if (IsMemberUnlocked(id))
		return false;

	CharacterDatabase::CharacterData& member = CharacterDatabase::Instance().GetCharacterData(id);

	members.emplace_back(&member);

	onMembersChanged.Trigger();
	return true;
}

bool Party::RemovePartyMemeber(int id)
{
	if (!IsMemberUnlocked(id))
		return false;
	if (!IsMemberInParty(id))
		return false;

	auto it = std::find_if(party.begin(), party.end(), [id](const CharacterDatabase::CharacterData* member) { return member->id == id; });
	if (it != party.end())
	{
		if (IsPartyLeader(id)) {
			leader = party[0];
		}
		party.erase(it);
		if (party.size() == 1)
			leader = party[0];
		onPartyChanged.Trigger();
		return true;
	}
	else
		return false; 
}

bool Party::RemoveMemeber(int id)
{
	if (!IsMemberUnlocked(id))
		return false;

	auto it = std::find_if(members.begin(), members.end(), [id](const CharacterDatabase::CharacterData* member) { return member->id == id; });
	if (it != members.end())
	{
		RemovePartyMemeber(id);

		members.erase(it);
		onMembersChanged.Trigger();
		return true;
	}
	else
		return false;
}

bool Party::SetPartyLeader(int id)
{
	if (!IsMemberInParty(id))
		return false;

	leader = &CharacterDatabase::Instance().GetCharacterData(id);
	onPartyChanged.Trigger();
	return true;
}

bool Party::EditPartyMember(int index, int id)
{
	if (IsMemberInParty(id))
		return false;
	if (!IsMemberUnlocked(id))
		return false;


	if (index >= party.size())
		return false;

	CharacterDatabase::CharacterData& member = CharacterDatabase::Instance().GetCharacterData(id);
	if (IsPartyLeader(party[index]->id)) 
		leader = &member;
	party[index] = &member;

	onPartyChanged.Trigger();
    return true;
}

bool Party::SwapPartyMembers(int id, int id2)
{
	if(!IsMemberInParty(id) || !IsMemberInParty(id2))
		return false;

	if (!IsMemberUnlocked(id) || !IsMemberUnlocked(id2))
		return false;


	auto it1 = std::find_if(party.begin(), party.end(), [id](const CharacterDatabase::CharacterData* member) { return member->id == id; });
	auto it2 = std::find_if(party.begin(), party.end(), [id2](const CharacterDatabase::CharacterData* member) { return member->id == id2; });
	if (it1 != party.end() && it2 != party.end())
	{

		if (IsPartyLeader((*it1)->id)) {
			SetPartyLeader((*it2)->id);
		}
		else if (IsPartyLeader((*it2)->id)) {
			SetPartyLeader((*it1)->id);
		}

		std::iter_swap(it1, it2);
		onPartyChanged.Trigger();
		return true;
	}
	else
		return false;
}

void Party::ClearParty()
{
	party.clear();
	onPartyChanged.Trigger();
}

void Party::ClearMemebers()
{
	members.clear();
	onMembersChanged.Trigger();

	ClearParty();
}

void Party::SetMaxPartySize(int max)
{
	partyMaxSize = max;
}

int Party::GetMaxPartySize() const
{
    return partyMaxSize;
}

int Party::GetPartySize() const
{
	return party.size();
}

int Party::GetMemebersAmount() const
{
	return members.size();
}


CharacterDatabase::CharacterData* Party::GetPartyLeader() const
{
	if (leader == nullptr)
		throw std::runtime_error("Leader is not set.");

	return leader;
}

vector<CharacterDatabase::CharacterData*> Party::GetParty(bool removeLeader) const
{
	if (removeLeader) {
		vector<CharacterDatabase::CharacterData*> copy = party;
		auto it = std::remove_if(copy.begin(), copy.end(), [this](const CharacterDatabase::CharacterData* member) { return member->id == leader->id; });
		copy.erase(it, copy.end());
		return copy;
	}

	return party;
}

vector<CharacterDatabase::CharacterData*> Party::GetMemebers() const
{
    return members;
}

int Party::GetPartyLeaderId() const
{
	return GetPartyLeader()->id;
}

vector<int> Party::GetPartyIds(bool removeLeader) const
{
	vector<int> ids;
	for (size_t i = 0; i < party.size(); i++)
	{
		ids.emplace_back(party[i]->id);
	}

	if (removeLeader) {
		auto it = std::remove(ids.begin(), ids.end(), GetPartyLeader()->id);
		ids.erase(it, ids.end());
	}

	return ids;
}

vector<int> Party::GetMemebersIds() const
{
	vector<int> ids;
	for (size_t i = 0; i < members.size(); i++)
	{
		ids.emplace_back(members[i]->id);
	}
	return ids;
}

CharacterDatabase::CharacterData* Party::GetCharacterFromParty(int index) const
{
	return party[index];
}

bool Party::IsPartyLeader(int id) const
{
	return GetPartyLeader()->id == id;
}

bool Party::IsMemberInParty(int id) const
{
	for (const auto& member : party)
	{
		if (member->id == id)
			return true;
	}

    return false;
}

bool Party::IsMemberUnlocked(int id) const
{
	for (const auto& member : members)
	{
		if (member->id == id)
			return true;
	}

	return false;
}
