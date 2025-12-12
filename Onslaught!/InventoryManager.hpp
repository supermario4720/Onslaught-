#pragma once
#include "Item.hpp"
#include "ItemData.hpp"
#include <vector>
#include <iostream>

struct InventorySlot {
    ItemID id = ItemID:None;
    int quantity = 0;
};

class InventoryManager {
public:
    InventoryManager(int maxSlots = 20)
    : maxInventorySlots(maxSlots), inventory(maxSlots), isInventoryFull(false) {}

    int addItem(ItemID id, int amount) {
        if (id == ItemID::None || amound <= 0) return 0;
        const bool stackable = ItemDatabase::get(id).stackable;
        const int maxStack = ItemDatabase::get(id).maxStack;

        int remaining = amount;

        if(stackable) {
            for(auto& slot : slots) {
                // if item is the and less than maxStack
                if(slot.id == id && slot.quantity < maxStack) {
                    const int canAdd = std::min(remaining, (maxStack - slot.quantity) );
                    slot.quantity += canAdd;
                    remaining -= canAdd;
                    // if all the items are added, return
                    if (remaining == 0) return amount;
                }
            }
        }
        // if items are still remaining, add them to new stack
        for(auto& slot : slots) {
            if(remaining == 0) break;
            if (slot.quantity == 0) {
                slot.id = id;
                const int toPlace = stackable ? std::min(remaining, maxStack) : 1;
                slot.quantity = toPlace;
                remaining -= toPlace;
            }
        }

        isInventoryFull = !hasEmptySlot();
        return amount - remaining;

    }

    void reset() {
        for (auto& slot : slots) {
            slot.id = ItemID::None;
            slot.quantity = 0;
        }
        isInventoryFull = false;
    }

private:
    bool hasEmptySlot() const {
        // returns true if there are any slots with nothing inside
        for (const auto& slot : slots) {
            if (slot.quantity == 0) return true;
        }
        return false;
    }
    int maxInventorySlots;
    bool isInventoryFull;
    std::vector<InventorySlot> inventory;
};
