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

    void displayInventory() const {
        cout << "Inventory (" << GetUsedSlots() << "/" << size << " slots used):\n";
        for (size_t i = 0; i < slots.size(); ++i) {
            const auto& slot = slots[i];
            cout << "[" << i + 1 << "] ";
            if (slot.IsEmpty()) {
                cout << "Empty";
            }
            else {
                cout << slot.item->GetName()
                    << " x" << slot.count
                    << " (Max " << slot.item->GetMaxStack() << ")";
            }
            cout << "\n";
        }
    }

public:

private:

private:
    vector<InventorySlot> slots;
    int size;
protected:

protected:

};