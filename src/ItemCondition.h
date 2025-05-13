#pragma once
#include "MissionCondition.h"
#include "Inventory.h"
#include <string>

using namespace std;

class ItemCondition : public MissionCondition {
public:
	ItemCondition(string _itemId, int _amount, Inventory* _inventoryToTrak);
	~ItemCondition() override = default;

	void SetUp() override;

	bool Check() override;
public:

private:

private:

	Inventory* inventoryToTrak = nullptr;
	string itemId = "";
	int amount = 0;
};