#pragma once
#include <string>

using namespace std;

class InventoryItem {
public:

    virtual ~InventoryItem() = default;

    string GetName() const;
    int GetMaxStack() const;

    virtual InventoryItem* clone() const = 0;

    virtual void Use() = 0;

public:
	
private:

private:

protected:

protected:
    std::string name;
    int maxStack = 1;
};