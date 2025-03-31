#pragma once
#include "Attack.h"
#include <string>

using namespace std;
class AttackList
{
public:
	AttackList();
	~AttackList();
	Attack* GetAttack(string _name);
	Attack* GetAttack(int ID);
private:
	void LoadAttacks();
	vector<Attack> attackList;
	string path = "Assets/Data/AttackList.xml";
};