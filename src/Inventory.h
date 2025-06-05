#pragma once
#include "InventoryItem.h"
#include "SystemEvent.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct InventorySlot {
    InventoryItem* item = nullptr;
    int count = 0;
    string slotType = "";

    void SetSlotType(const string& type) {
        slotType = type;
    }
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

    void SwapSlots(int index1, int index2, Inventory* targetInventory = nullptr);
    bool TryStackItems(int sourceIndex, int targetIndex, Inventory* targetInventory = nullptr);

    bool HasItem(string item_id, int amount = 1);
    bool CanAddItem(string item_id, int amount = 1);

    void ClearAllItems();
    const vector<InventorySlot>& GetSlotsData();
    vector<InventorySlot>& GetSlotsDataModifiable();

    SystemEvent<> onInventoryChanged;

private:
    vector<InventorySlot> slots;
    int size;

    static void SwapSlotContents(InventorySlot& a, InventorySlot& b);
    bool CanPlaceItemInSlot(const InventoryItem* item, const InventorySlot& slot) const;
};