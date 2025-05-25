#pragma once
#include "MissionCondition.h"
#include "Inventory.h"
#include <string>
#include <vector>

using namespace std;

class ItemCondition : public MissionCondition {
public:
	ItemCondition(vector<string> _itemId, vector<int> _amount, Inventory* _inventoryToTrak);
	~ItemCondition() override = default;

	void SetUp() override;

	bool Check() override;
public:

private:

private:

	Inventory* inventoryToTrak = nullptr;
	vector<string> itemsId;
	vector<int> amounts;
	vector<bool> tasks;
};