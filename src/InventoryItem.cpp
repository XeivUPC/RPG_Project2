#include "InventoryItem.h"

InventoryItem::InventoryItem(Item* _itemRef)
{
    itemRef = _itemRef;
}

string InventoryItem::GetName() const
{
    return itemRef->name;
}

string InventoryItem::GetId() const
{
    return itemRef->id;
}

int InventoryItem::GetMaxStack() const
{
    return itemRef->stackable_quantity;
}

string InventoryItem::GetType() const
{
    if (itemRef->properties.count("type") == 0)
        return "";
    return itemRef->properties["type"];
}

InventoryItem* InventoryItem::clone() const
{
    return new InventoryItem(*this);
}

Item* InventoryItem::GetReference()
{
    return itemRef;
}

void InventoryItem::Use()
{
}
