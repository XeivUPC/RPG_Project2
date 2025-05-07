#pragma once
#include <string>
#include "Item.h"

using namespace std;

class InventoryItem {
public:

    InventoryItem(Item* _itemRef);
    ~InventoryItem() = default;

    string GetName() const;
    int GetMaxStack() const;

    InventoryItem* clone() const;

    void Use();

public:
	
private:

private:

protected:

protected:
    Item* itemRef;
};