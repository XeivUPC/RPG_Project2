#include "InventoryItem.h"

InventoryItem::InventoryItem(Item* _itemRef)
{
    itemRef = _itemRef;
}

string InventoryItem::GetName() const
{
    return itemRef->name;
}

int InventoryItem::GetMaxStack() const
{
    return itemRef->stackable_quantity;
}

InventoryItem* InventoryItem::clone() const
{
    return new InventoryItem(*this);
}

void InventoryItem::Use()
{
}
