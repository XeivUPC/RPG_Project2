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

void Inventory::SwapSlots(int index1, int index2, Inventory* targetInventory) {
    if (targetInventory == nullptr) {
        if (index1 < 0 || index1 >= size || index2 < 0 || index2 >= size) {
            return;
        }
        std::swap(slots[index1], slots[index2]);
    }
    else {
        if (index1 < 0 || index1 >= size || index2 < 0 || index2 >= targetInventory->size) {
            return;
        }
        std::swap(slots[index1], targetInventory->slots[index2]);
    }
}

bool Inventory::TryStackItems(int sourceIndex, int targetIndex, Inventory* targetInventory) {
    if (targetInventory == nullptr) {
        if (sourceIndex < 0 || sourceIndex >= size ||
            targetIndex < 0 || targetIndex >= size ||
            sourceIndex == targetIndex) {
            return false;
        }

        InventorySlot& source = slots[sourceIndex];
        InventorySlot& target = slots[targetIndex];

        if (source.IsEmpty() || target.IsEmpty() ||
            source.item->GetName() != target.item->GetName()) {
            return false;
        }

        int maxStack = source.item->GetMaxStack();
        if (target.count >= maxStack) {
            return false;
        }

        int amountToTransfer = std::min(source.count, maxStack - target.count);
        target.count += amountToTransfer;
        source.count -= amountToTransfer;

        if (source.count <= 0) {
            delete source.item;
            source.item = nullptr;
            source.count = 0;
        }

        return true;
    }
    else {
        if (sourceIndex < 0 || sourceIndex >= size ||
            targetIndex < 0 || targetIndex >= targetInventory->size) {
            return false;
        }

        InventorySlot& source = slots[sourceIndex];
        InventorySlot& target = targetInventory->slots[targetIndex];

        if (source.IsEmpty() || target.IsEmpty() ||
            source.item->GetName() != target.item->GetName()) {
            return false;
        }

        int maxStack = source.item->GetMaxStack();
        if (target.count >= maxStack) {
            return false;
        }

        int amountToTransfer = std::min(source.count, maxStack - target.count);
        target.count += amountToTransfer;
        source.count -= amountToTransfer;

        if (source.count <= 0) {
            delete source.item;
            source.item = nullptr;
            source.count = 0;
        }

        return true;
    }
}

void Inventory::ClearAllItems()
{
    for (auto& slot : slots) {
        if (!slot.IsEmpty()) {
            delete slot.item;
            slot.item = nullptr;
            slot.count = 0;
        }
    }
}

const vector<InventorySlot>& Inventory::GetSlotsData()
{
    return slots;
}

vector<InventorySlot>& Inventory::GetSlotsDataModifiable()
{
    return slots;
}
