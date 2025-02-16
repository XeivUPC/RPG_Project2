#include "InventoryItem.h"

string InventoryItem::GetName() const
{
    return name;
}

int InventoryItem::GetMaxStack() const
{
    return maxStack;
}
