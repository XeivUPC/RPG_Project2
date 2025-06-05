#include "Inventory.h"
#include "ItemList.h"
#include <algorithm>
#include <iostream>

Inventory::Inventory(int size) : size(size) {
    slots.resize(size);
}

Inventory::~Inventory() {
    for (auto& slot : slots) {
        if (!slot.IsEmpty()) delete slot.item;
    }
    onInventoryChanged.UnsubscribeAll();
}

bool Inventory::CanPlaceItemInSlot(const InventoryItem* item, const InventorySlot& slot) const {
    // Empty slots with no type restriction
    if (slot.IsEmpty()) {
        return slot.slotType.empty() || slot.slotType == item->GetType();
    }
    // Non-empty slots must match type if slot has type restriction
    return slot.slotType.empty() || slot.slotType == item->GetType();
}

void Inventory::SwapSlotContents(InventorySlot& a, InventorySlot& b) {
    std::swap(a.item, b.item);
    std::swap(a.count, b.count);
}

int Inventory::AddItem(InventoryItem& newItem, int amount) {
    if (amount <= 0) return 0;

    int added = 0;
    const int maxStack = newItem.GetMaxStack();
    const std::string targetName = newItem.GetName();
    const std::string itemType = newItem.GetType();

    // First pass: stack on existing compatible slots
    for (auto& slot : slots) {
        if (!slot.IsEmpty() && slot.item->GetName() == targetName &&
            CanPlaceItemInSlot(&newItem, slot)) {
            const int canAdd = min(maxStack - slot.count, amount);
            slot.count += canAdd;
            added += canAdd;
            amount -= canAdd;

            if (amount == 0) {
                delete& newItem;
                onInventoryChanged.Trigger();
                return added;
            }
        }
    }

    // Second pass: use empty compatible slots
    for (auto& slot : slots) {
        if (slot.IsEmpty() && CanPlaceItemInSlot(&newItem, slot)) {
            slot.item = newItem.clone();
            const int canAdd = std::min(maxStack, amount);
            slot.count = canAdd;
            added += canAdd;
            amount -= canAdd;

            if (amount == 0) {
                delete& newItem;
                onInventoryChanged.Trigger();
                return added;
            }
        }
    }

    delete& newItem;
    onInventoryChanged.Trigger();
    return added;
}

void Inventory::RemoveItem(const std::string& itemName, int amount) {
    for (auto& slot : slots) {
        if (!slot.IsEmpty() && slot.item != nullptr && slot.item->GetName() == itemName) {
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
    onInventoryChanged.Trigger();
}

void Inventory::UseItem(const string& itemName, int amount) {
    RemoveItem(itemName, amount);
}

int Inventory::GetItemCount(const std::string& itemName) const {
    int total = 0;
    for (const auto& slot : slots) {
        if (!slot.IsEmpty() && slot.item->GetId() == itemName) {
            total += slot.count;
        }
    }
    return total;
}

int Inventory::GetTotalSlots() const {
    return size;
}

int Inventory::GetUsedSlots() const {
    return (int)(std::count_if(slots.begin(), slots.end(),
        [](const InventorySlot& s) { return !s.IsEmpty(); }));
}

int Inventory::GetFreeSlots() const {
    return size - GetUsedSlots();
}

void Inventory::SwapSlots(int index1, int index2, Inventory* targetInventory) {
    if (targetInventory == nullptr) {
        if (index1 < 0 || index1 >= size || index2 < 0 || index2 >= size) {
            return;
        }

        InventorySlot& slot1 = slots[index1];
        InventorySlot& slot2 = slots[index2];

        // Check if items can be placed in target slots
        if ((!slot1.IsEmpty() && !CanPlaceItemInSlot(slot1.item, slot2)) ||
            (!slot2.IsEmpty() && !CanPlaceItemInSlot(slot2.item, slot1))) {
            return;
        }

        SwapSlotContents(slot1, slot2);
    }
    else {
        if (index1 < 0 || index1 >= size || index2 < 0 || index2 >= targetInventory->size) {
            return;
        }

        InventorySlot& slot1 = slots[index1];
        InventorySlot& slot2 = targetInventory->slots[index2];

        // Check if items can be placed in target slots
        if ((!slot1.IsEmpty() && !CanPlaceItemInSlot(slot1.item, slot2)) ||
            (!slot2.IsEmpty() && !CanPlaceItemInSlot(slot2.item, slot1))) {
            return;
        }

        SwapSlotContents(slot1, slot2);
    }
    onInventoryChanged.Trigger();
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
            source.item->GetName() != target.item->GetName() ||
            !CanPlaceItemInSlot(source.item, target)) {
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

        onInventoryChanged.Trigger();
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
            source.item->GetName() != target.item->GetName() ||
            !CanPlaceItemInSlot(source.item, target)) {
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

        onInventoryChanged.Trigger();
        return true;
    }
}

bool Inventory::HasItem(string item_id, int amount) {
    return GetItemCount(item_id) >= amount;
}

bool Inventory::CanAddItem(string item_id, int amount) {
    if (amount <= 0)
        return true;

    Item* baseItem = ItemList::Instance().ItemByID(item_id);
    if (!baseItem) return false; 

    string itemType = "";
    auto it = baseItem->properties.find("type");
    if (it != baseItem->properties.end()) {
        itemType = it->second;
    }
    int maxStack = baseItem->stackable_quantity;

    int remaining = amount;

    for (const auto& slot : slots) {
        if (!slot.IsEmpty() &&
            slot.item->GetId() == item_id &&
            (slot.slotType.empty() || slot.slotType == itemType)) {

            int spaceInStack = slot.getRemainingSpace();
            if (spaceInStack > 0) {
                if (remaining <= spaceInStack) {
                    return true; 
                }
                remaining -= spaceInStack;
            }
        }
    }

    int availableEmptySlots = 0;
    for (const auto& slot : slots) {
        if (slot.IsEmpty() &&
            (slot.slotType.empty() || slot.slotType == itemType)) {
            availableEmptySlots++;
        }
    }

    int neededSlots = (remaining + maxStack - 1) / maxStack;
    return availableEmptySlots >= neededSlots;
}

void Inventory::ClearAllItems() {
    for (auto& slot : slots) {
        if (!slot.IsEmpty()) {
            delete slot.item;
            slot.item = nullptr;
            slot.count = 0;
        }
    }
    onInventoryChanged.Trigger();
}

const vector<InventorySlot>& Inventory::GetSlotsData() {
    return slots;
}

vector<InventorySlot>& Inventory::GetSlotsDataModifiable() {
    return slots;
}