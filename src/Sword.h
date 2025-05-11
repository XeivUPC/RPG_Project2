#pragma once
#include "InventoryItem.h"
#include <iostream>

class Sword : public InventoryItem {
public:
    Sword();
    InventoryItem* clone() const override;
    void Use() override;
};

Sword::Sword() {
    name = "Steel Sword";
    maxStack = 1;  // Non-stackable
}

InventoryItem* Sword::clone() const {
    return new Sword(*this);
}

void Sword::Use() {
    cout << "Swung " << name << "!\n";
}