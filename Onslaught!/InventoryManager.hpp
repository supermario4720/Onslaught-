#pragma once
#include "Item.hpp"
#include "ItemData.hpp"
#include <vector>

struct InventorySlot {
    ItemID id = ItemID::None;
    int quantity = 0;
    bool isFull = false;
};

class InventoryManager {
public:
    InventoryManager(int maxSlots = 20);

    int addItem(ItemID id, int amount);

    void reset();

    void printItems() const;

private:
    bool hasEmptySlot() const;
    bool isCompletelyFull() const;

    int maxInventorySlots;
    bool isInventoryFull;
    std::vector<InventorySlot> inventory;
};
