#pragma once
#include "InventoryItem.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct InventorySlot {
    InventoryItem* item = nullptr;
    int count = 0;

    bool IsEmpty() const { return item == nullptr; }
    int getRemainingSpace() const {
        return IsEmpty() ? 0 : (item->GetMaxStack() - count);
    }
};


class Inventory {
public:
    explicit Inventory(int size = 20);
	~Inventory();

    int AddItem(InventoryItem& item, int amount = 1);
    void RemoveItem(const string& itemName, int amount = 1);
    void UseItem(const string& itemName, int amount = 1);

    int GetItemCount(const string& itemName) const;

    int GetTotalSlots() const;
    int GetUsedSlots() const;
    int GetFreeSlots() const;

    void SwapSlots(int index1, int index2);
    bool TryStackItems(int sourceIndex, int targetIndex);

    void ClearAllItems();

    const vector<InventorySlot>& GetSlotsData();
    vector<InventorySlot>& GetSlotsDataModifiable();

public:

private:

private:
    vector<InventorySlot> slots;
    int size;
protected:

protected:

};