#include "AttackList.h"

AttackList::AttackList()
{
	LoadAttacks();
}

AttackList::~AttackList()
{
	attackList.clear();
}
