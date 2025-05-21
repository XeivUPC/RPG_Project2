#include "Party.h"

#include <algorithm>

Party::Party(string defaultId)
{
	AddMemeber(defaultId);
	AddPartyMemeber(defaultId);
	SetPartyLeader();
}

Party::~Party()
{
	party.clear();
	leader = nullptr;
}

bool Party::AddPartyMemeber(string id)
{
	if (!IsMemberUnlocked(id))
		return false;
	if (IsMemberInParty(id))
		return false;

	if (party.size() >= partyMaxSize)
		return false;

	CharacterDatabase::CharacterDefinition& member = CharacterDatabase::Instance().GetCharacterDefinition(id);

	party.emplace_back(&member);

	onPartyChanged.Trigger();
    return true;
}

bool Party::AddMemeber(string id)
{
	if (IsMemberUnlocked(id))
		return false;

	CharacterDatabase::CharacterDefinition& member = CharacterDatabase::Instance().GetCharacterDefinition(id);

	members.emplace_back(&member);
	SortMemebers();
	onMembersChanged.Trigger();
	return true;
}

bool Party::RemovePartyMemeber(string id)
{
	if (!IsMemberUnlocked(id))
		return false;
	if (!IsMemberInParty(id))
		return false;

	if (party.size() <= 1)
		return false;

	auto it = std::find_if(party.begin(), party.end(), [id](const CharacterDatabase::CharacterDefinition* member) { return member->id == id; });
	if (it != party.end())
	{
		party.erase(it);
		SetPartyLeader();
		onPartyChanged.Trigger();
		return true;
	}
	else
		return false; 
}

bool Party::RemoveMemeber(string id)
{
	if (!IsMemberUnlocked(id))
		return false;

	auto it = std::find_if(members.begin(), members.end(), [id](const CharacterDatabase::CharacterDefinition* member) { return member->id == id; });
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

bool Party::SetPartyLeader()
{
	if (leader == party[0])
		return true;
	leader = party[0];
	onPartyChanged.Trigger();
	return true;
}

bool Party::EditPartyMember(int index, string id)
{
	if (IsMemberInParty(id))
		return false;
	if (!IsMemberUnlocked(id))
		return false;


	if (index >= party.size())
		return false;

	CharacterDatabase::CharacterDefinition& member = CharacterDatabase::Instance().GetCharacterDefinition(id);
	party[index] = &member;
	SetPartyLeader();

	onPartyChanged.Trigger();
    return true;
}

bool Party::SwapPartyMembers(string id, string id2)
{
	if(!IsMemberInParty(id) || !IsMemberInParty(id2))
		return false;

	if (!IsMemberUnlocked(id) || !IsMemberUnlocked(id2))
		return false;


	auto it1 = std::find_if(party.begin(), party.end(), [id](const CharacterDatabase::CharacterDefinition* member) { return member->id == id; });
	auto it2 = std::find_if(party.begin(), party.end(), [id2](const CharacterDatabase::CharacterDefinition* member) { return member->id == id2; });
	if (it1 != party.end() && it2 != party.end())
	{

		std::iter_swap(it1, it2);
		SetPartyLeader();
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
	return (int)party.size();
}

int Party::GetMemebersAmount() const
{
	return (int)members.size();
}


CharacterDatabase::CharacterDefinition* Party::GetPartyLeader() const
{
	if (leader == nullptr)
		throw std::runtime_error("Leader is not set.");

	return leader;
}

vector<CharacterDatabase::CharacterDefinition*> Party::GetParty(bool removeLeader) const
{
	if (removeLeader) {
		vector<CharacterDatabase::CharacterDefinition*> copy = party;
		auto it = std::remove_if(copy.begin(), copy.end(), [this](const CharacterDatabase::CharacterDefinition* member) { return member->id == leader->id; });
		copy.erase(it, copy.end());
		return copy;
	}

	return party;
}

vector<CharacterDatabase::CharacterDefinition*> Party::GetMemebers() const
{
    return members;
}

string Party::GetPartyLeaderId() const
{
	return GetPartyLeader()->id;
}

vector<string> Party::GetPartyIds(bool removeLeader) const
{
	vector<string> ids;
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

vector<string> Party::GetMembersIds() const
{
	vector<string> ids;
	for (size_t i = 0; i < members.size(); i++)
	{
		ids.emplace_back(members[i]->id);
	}
	return ids;
}

CharacterDatabase::CharacterDefinition* Party::GetCharacterFromParty(int index) const
{
	return party[index];
}

CharacterDatabase::CharacterDefinition* Party::GetCharacterFromMembers(string id) const
{

	CharacterDatabase::CharacterDefinition* memberPtr = nullptr;

	for (const auto& member : members)
	{
		if (member->id == id)
		{
			memberPtr = member;
			break;
		}
	}
	return memberPtr;
}

bool Party::IsPartyLeader(string id) const
{
	return GetPartyLeader()->id == id;
}

void Party::SortMemebers()
{
	std::sort(members.begin(), members.end(), [](const CharacterDatabase::CharacterDefinition* a, const CharacterDatabase::CharacterDefinition* b) {
		return a->id < b->id;
		});
}

bool Party::IsMemberInParty(string id) const
{
	for (const auto& member : party)
	{
		if (member->id == id)
			return true;
	}

    return false;
}

bool Party::IsMemberUnlocked(string id) const
{
	for (const auto& member : members)
	{
		if (member->id == id)
			return true;
	}

	return false;
}
