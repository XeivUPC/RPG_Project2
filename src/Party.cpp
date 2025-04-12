#include "Party.h"

#include <algorithm>

Party::Party(int defaultId)
{
	AddMember(defaultId);
}

Party::~Party()
{
	members.clear();
	leader = nullptr;
}

bool Party::AddMember(int id)
{
	if (IsMemberInParty(id))
		return false;
	if (members.size() >= maxMembers)
		return false;

	CharacterDatabase::CharacterData& member = CharacterDatabase::Instance().GetCharacterData(id);

	members.emplace_back(&member);
	if (members.size() == 1)
		leader = &member;

	onPartyChanged.Trigger();
    return true;
}

bool Party::RemoveMember(int id)
{
	if (!IsMemberInParty(id))
		return false;

	auto it = std::find_if(members.begin(), members.end(), [id](const CharacterDatabase::CharacterData* member) { return member->id == id; });
	if (it != members.end())
	{
		if (leader == (*it))
		{
			leader = nullptr;
			if (members.size() > 1)
				leader = members[0];
		}
		members.erase(it);
		onPartyChanged.Trigger();
		return true;
	}
	else
		return false;

   
}

bool Party::SetLeader(int index)
{
	if (index >= members.size())
		return false;

	leader = members[index];
	onPartyChanged.Trigger();
	return true;
}

bool Party::EditMember(int index, int id)
{
	if (IsMemberInParty(id))
		return false;
	if (index >= members.size())
		return false;

	CharacterDatabase::CharacterData& member = CharacterDatabase::Instance().GetCharacterData(id);
	members[index] = &member;

	onPartyChanged.Trigger();
    return true;
}

bool Party::SwapMembers(int id, int id2)
{
	if(!IsMemberInParty(id) || !IsMemberInParty(id2))
		return false;

	auto it1 = std::find_if(members.begin(), members.end(), [id](const CharacterDatabase::CharacterData* member) { return member->id == id; });
	auto it2 = std::find_if(members.begin(), members.end(), [id2](const CharacterDatabase::CharacterData* member) { return member->id == id2; });
	if (it1 != members.end() && it2 != members.end())
	{
		std::iter_swap(it1, it2);
		onPartyChanged.Trigger();
		return true;
	}
	else
		return false;
}

void Party::ClearParty()
{
	members.clear();
	if(leader!=nullptr)
		members.emplace_back(leader);
	onPartyChanged.Trigger();
}

void Party::SetMaxMembers(int max)
{
	maxMembers = max;
}

int Party::GetMaxMembers() const
{
    return maxMembers;
}

int Party::GetCurrentMembers() const
{
	return members.size();
}

CharacterDatabase::CharacterData* Party::GetLeader() const
{
	if (leader == nullptr)
		throw std::runtime_error("Leader is not set.");

	return leader;
}

vector<CharacterDatabase::CharacterData*> Party::GetMembers() const
{
    return members;
}

CharacterDatabase::CharacterData* Party::GetMember(int index) const
{
	return members[index];
}

bool Party::IsMemberInParty(int id) const
{
	for (const auto& member : members)
	{
		if (member->id == id)
			return true;
	}

    return false;
}
