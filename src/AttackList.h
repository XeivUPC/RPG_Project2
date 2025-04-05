#pragma once
#include "Attack.h"
#include <string>
#include <vector>

using namespace std;

class AttackList
{
public:

	static AttackList& Instance() {
		static AttackList instance;
		return instance;
	}
	
	Attack* GetAttack(string _name);
	Attack* GetAttack(int ID);
private:
	AttackList();
	~AttackList();

	AttackList(const AttackList&) = delete;
	AttackList& operator=(const AttackList&) = delete;

	void LoadAttacks();
	vector<Attack> attackList;
	string path = "Assets/Data/AttackList.xml";
};