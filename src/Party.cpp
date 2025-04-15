#include "Party.h"

#include <algorithm>

Party::Party(int defaultId)
{
	SetLeader(defaultId);
}

Party::~Party()
{
	followers.clear();
	leader = nullptr;
}

bool Party::AddFollower(int id)
{
	if (IsMemberInParty(id))
		return false;
	if (followers.size() >= followersMaxAmount)
		return false;

	CharacterDatabase::CharacterData& member = CharacterDatabase::Instance().GetCharacterData(id);

	followers.emplace_back(&member);

	onPartyChanged.Trigger();
    return true;
}

bool Party::RemoveFollower(int id)
{
	if (!IsMemberInParty(id))
		return false;

	auto it = std::find_if(followers.begin(), followers.end(), [id](const CharacterDatabase::CharacterData* member) { return member->id == id; });
	if (it != followers.end())
	{
		followers.erase(it);
		onPartyChanged.Trigger();
		return true;
	}
	else
		return false;

   
}

bool Party::SetLeader(int id)
{
	leader = &CharacterDatabase::Instance().GetCharacterData(id);;
	onPartyChanged.Trigger();
	return true;
}

bool Party::EditMember(int index, int id)
{
	if (IsMemberInParty(id))
		return false;
	if (index >= followers.size())
		return false;

	CharacterDatabase::CharacterData& member = CharacterDatabase::Instance().GetCharacterData(id);
	followers[index] = &member;

	onPartyChanged.Trigger();
    return true;
}

bool Party::SwapMembers(int id, int id2)
{
	if(!IsMemberInParty(id) || !IsMemberInParty(id2))
		return false;

	if (leader->id == id || leader->id == id2) {
		int idToFind = leader->id == id ? id2 : id;
		CharacterDatabase::CharacterData* aux = leader;
		for (size_t i = 0; i < followers.size(); i++)
		{
			if (followers[i]->id == idToFind) {
				leader = followers[i];
				followers[i] = aux;
				onPartyChanged.Trigger();
				return true;
			}
		}
		return false;
	}

	auto it1 = std::find_if(followers.begin(), followers.end(), [id](const CharacterDatabase::CharacterData* member) { return member->id == id; });
	auto it2 = std::find_if(followers.begin(), followers.end(), [id2](const CharacterDatabase::CharacterData* member) { return member->id == id2; });
	if (it1 != followers.end() && it2 != followers.end())
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
	followers.clear();
	onPartyChanged.Trigger();
}

void Party::SetMaxPartySize(int max)
{
	followersMaxAmount = max;
}

int Party::GetMaxPartySize() const
{
    return followersMaxAmount;
}

int Party::GetPartySize() const
{
	return followers.size() + 1;
}

int Party::GetFollowersAmount() const
{
	return followers.size();
}


CharacterDatabase::CharacterData* Party::GetLeader() const
{
	if (leader == nullptr)
		throw std::runtime_error("Leader is not set.");

	return leader;
}

vector<CharacterDatabase::CharacterData*> Party::GetParty() const
{
	vector<CharacterDatabase::CharacterData*> party;
	party.emplace_back(leader);
	for (size_t i = 0; i < followers.size(); i++)
	{
		party.emplace_back(followers[i]);
	}
	
	return party;
}

vector<CharacterDatabase::CharacterData*> Party::GetFollowers() const
{
    return followers;
}

int Party::GetLeaderId() const
{
	return GetLeader()->id;
}

vector<int> Party::GetActivePartyIds()
{
	vector<int> ids;
	ids.emplace_back(leader->id);
	for (size_t i = 0; i < followers.size(); i++)
	{
		ids.emplace_back(followers[i]->id);
	}
	return ids;
}

vector<int> Party::GetActiveFollowersIds() const
{
	vector<int> ids;
	for (size_t i = 0; i < followers.size(); i++)
	{
		ids.emplace_back(followers[i]->id);
	}
	return ids;
}

CharacterDatabase::CharacterData* Party::GetMember(int index) const
{
	return followers[index];
}

bool Party::IsMemberInParty(int id) const
{
	if (leader->id == id)
		return true;
	for (const auto& member : followers)
	{
		if (member->id == id)
			return true;
	}

    return false;
}
