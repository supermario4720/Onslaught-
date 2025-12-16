#include "InventoryManager.hpp"
#include <vector>
#include <iostream>


InventoryManager::InventoryManager(int maxSlots)
: maxInventorySlots(maxSlots), inventory(maxSlots), isInventoryFull(false) {}

// adds item to inventory if space remains. Returns the remaining item amount
int InventoryManager::addItem(ItemID id, int amount) {
    printItems();
    if (id == ItemID::None || amount <= 0) return 0;
    const bool stackable = ItemDatabase::get(id).stackable;
    const int maxStack = ItemDatabase::get(id).maxStack;

    int remaining = amount;
    // if all slots completely full, return
    if(isInventoryFull) return remaining;

    if(stackable) {
        for(auto& slot : inventory) {
            // if item is the and less than maxStack
            if(slot.id == id && !slot.isFull) {
                const int canAdd = std::min(remaining, (maxStack - slot.quantity) );
                slot.quantity += canAdd;
                remaining -= canAdd;
                // if all the items are added, return
                if (slot.quantity == maxStack) slot.isFull = true;
                if (remaining == 0) return remaining;
            }
        }
    }
    // if items are still remaining and unstackable, add them to new stack
    for(auto& slot : inventory) {
        if(remaining == 0) break;
        if (!slot.isFull) {
            slot.id = id;
            const int toPlace = stackable ? std::min(remaining, maxStack) : 1;
            slot.quantity = toPlace;
            remaining -= toPlace;
            if (slot.quantity == maxStack) slot.isFull = true;
        }
    }

    isInventoryFull = isCompletelyFull();

    printItems();

    return amount - remaining;
}

void InventoryManager::reset() {
    for (auto& slot : inventory) {
        slot.id = ItemID::None;
        slot.quantity = 0;
    }
    isInventoryFull = false;
}

bool InventoryManager::hasEmptySlot() const {
    // returns true if there are any slots with nothing inside
    for (const auto& slot : inventory) {
        if (slot.quantity == 0) return true;
    }
    return false;
}

bool InventoryManager::isCompletelyFull() const {
    for (const auto& slot : inventory) {
        if (!slot.isFull) return false;
    }
    return true;
}

void InventoryManager::printItems() const {
    for (const auto& slot : inventory) {
        std::cout << ItemDatabase::get(slot.id).name << ": " << slot.quantity << std::endl;
    }
}