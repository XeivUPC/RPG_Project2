#pragma once
#include "InventoryItem.h"
#include <iostream>

class HealthPotion : public InventoryItem {
public:
    HealthPotion();
    InventoryItem* clone() const override;
    void Use() override;
};

HealthPotion::HealthPotion() {
    name = "Health Potion";

    maxStack = 10;
}

InventoryItem* HealthPotion::clone() const {
    return new HealthPotion(*this);
}

void HealthPotion::Use() {
    cout << "Used " << name << " to heal 50 HP\n";
}