#include "AttackList.h"

AttackList::AttackList()
{
	LoadAttacks();
}

AttackList::~AttackList()
{
	attackList.clear();
}

Attack* AttackList::GetAttack(string _name)
{
	for (size_t i = 0; i < attackList.size(); i++)
	{
		if (attackList[i].name == _name)
		{
			return &attackList[i];
		}
	}
	return nullptr;
}

Attack* AttackList::GetAttack(int ID)
{
	return &attackList[ID];
}

void AttackList::LoadAttacks()
{

}
