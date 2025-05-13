#include "ItemCondition.h"
#include "MissionManager.h"

ItemCondition::ItemCondition(string _itemId, int _amount, Inventory* _inventoryToTrak)
{
    itemId = _itemId;
    amount = _amount;
    inventoryToTrak = _inventoryToTrak;

    Check();
}

void ItemCondition::SetUp()
{

}

bool ItemCondition::Check()
{
    isDone = inventoryToTrak->HasItem(itemId, amount);
    return isDone;
}
