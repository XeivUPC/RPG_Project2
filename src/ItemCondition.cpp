#include "ItemCondition.h"
#include "MissionManager.h"

#include "Engine.h"
#include "GameScene.h"
#include "PlayerCharacter.h"
#include "Inventory.h"


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
    if (inventoryToTrak == nullptr) {
        isDone = Engine::Instance().s_game->GetPlayer()->inventory->HasItem(itemId, amount);
        return isDone;
    }
    isDone = inventoryToTrak->HasItem(itemId, amount);
    return isDone;
}
