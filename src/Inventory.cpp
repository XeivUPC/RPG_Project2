#include "Inventory.h"

#include <algorithm>
#include <iostream>

Inventory::Inventory(int size) : size(size) {
    slots.resize(size);
}

Inventory::~Inventory() {
    for (auto& slot : slots) {
        if (!slot.IsEmpty()) delete slot.item;
    }
}

int Inventory::AddItem(InventoryItem& newItem, int amount) {
    if (amount <= 0) return 0;

    int added = 0;
    const int maxStack = newItem.GetMaxStack();
    const std::string targetName = newItem.GetName();

    // First pass: stack into existing slots
    for (auto& slot : slots) {
        if (!slot.IsEmpty() && slot.item->GetName() == targetName) {
            const int canAdd = min(maxStack - slot.count, amount);
            slot.count += canAdd;
            added += canAdd;
            amount -= canAdd;

            if (amount == 0) {
                delete &newItem;
                return added;
            }
        }
    }

    // Second pass: use empty slots
    for (auto& slot : slots) {
        if (slot.IsEmpty()) {
            slot.item = newItem.clone();
            const int canAdd = std::min(maxStack, amount);
            slot.count = canAdd;
            added += canAdd;
            amount -= canAdd;

            if (amount == 0) {
                delete &newItem;
                return added;
            }
        }
    }

    delete &newItem;
    return added;
}

void Inventory::RemoveItem(const std::string& itemName, int amount) {
    for (auto& slot : slots) {
        if (!slot.IsEmpty() && slot.item!=nullptr && slot.item->GetName() == itemName) {
            const int removeAmount = min(slot.count, amount);
            slot.count -= removeAmount;
            amount -= removeAmount;

            if (slot.count <= 0) {
                delete slot.item;
                slot.item = nullptr;
                slot.count = 0;
            }

            if (amount <= 0) break;
        }
    }
}

void Inventory::UseItem(const string& itemName, int amount)
{
    RemoveItem(itemName, amount);
}

int Inventory::GetItemCount(const std::string& itemName) const {
    int total = 0;
    for (const auto& slot : slots) {
        if (!slot.IsEmpty() && slot.item->GetName() == itemName) {
            total += slot.count;
        }
    }
    return total;
}

int Inventory::GetTotalSlots() const
{
    return size;
}

int Inventory::GetUsedSlots() const {
    return (int)(std::count_if(slots.begin(), slots.end(),
        [](const InventorySlot& s) { return !s.IsEmpty(); }));
}

int Inventory::GetFreeSlots() const
{
    return size - GetUsedSlots();
}
