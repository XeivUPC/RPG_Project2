#include "ItemCondition.h"
#include "MissionManager.h"

#include "Engine.h"
#include "GameScene.h"
#include "PlayerCharacter.h"
#include "Inventory.h"


ItemCondition::ItemCondition(vector<string> _itemId, vector<int> _amount, Inventory* _inventoryToTrak)
{
    itemsId = _itemId;
    amounts = _amount;

    for (size_t i = 0; i < itemsId.size(); i++)
    {
        tasks.emplace_back(false);
    }

    inventoryToTrak = _inventoryToTrak;

    Check();
}

void ItemCondition::SetUp()
{

}

bool ItemCondition::Check()
{
    if (Completed())
        return true;

    for (size_t i = 0; i < tasks.size(); i++)
    {
        if (inventoryToTrak == nullptr) {
			tasks[i] = Engine::Instance().s_game->GetPlayer()->inventory->HasItem(itemsId[i], amounts[i]);
        }else
            tasks[i] = inventoryToTrak->HasItem(itemsId[i], amounts[i]);
    }


    isDone = true;
    for (size_t i = 0; i < tasks.size(); i++)
    {
        if (!tasks[i]) {
            isDone = false;
            break;
        }
    }

    return isDone;
}
