#include "InventoryManager.hpp"
#include <vector>
#include <iostream>


InventoryManager::InventoryManager(int maxSlots)
: maxInventorySlots(maxSlots), inventory(maxSlots), isInventoryFull(false) {
}

// adds item to inventory if space remains. Returns the remaining item amount
int InventoryManager::addItem(ItemID id, int amount) {
    if (id == ItemID::None || amount <= 0) return 0;
    const bool stackable = ItemDatabase::get(id).stackable;
    const int maxStack = ItemDatabase::get(id).maxStack;

    int remaining = amount;
    // if all slots completely full, return
    if(isInventoryFull) return remaining;

    if(stackable) {
        for(auto& slot : inventory) {
            // First, check slots with the same item and not full
            if(slot.id == id && !slot.isFull) {
                const int canAdd = std::min(remaining, (maxStack - slot.quantity) );
                slot.quantity += canAdd;
                slot.qtyText.setString(std::to_string(slot.quantity));
                remaining -= canAdd;
                // if all the items are added, return
                if (slot.quantity == maxStack) slot.isFull = true;
                if (remaining == 0) {
                    isInventoryFull = isCompletelyFull();
                    return remaining;
                }
            }
        }
        // If items still remain, loop through inventory again, using new inventory slots
        for(auto& slot : inventory) {
            // First, check slots with the same item and not full
            if(slot.id == ItemID::None) {
                slot.id = id;
                slot.sprite.setTexture( TextureManager::getInstance().getTexture(id) );
                const int canAdd = std::min(remaining, (maxStack - slot.quantity) );
                slot.quantity += canAdd;
                slot.qtyText.setString(std::to_string(slot.quantity));
                remaining -= canAdd;
                // if all the items are added, return
                if (slot.quantity == maxStack) slot.isFull = true;
                if (remaining == 0) {
                    isInventoryFull = isCompletelyFull();
                    return remaining;
                }
            }
        }
    }
    else {
        // for unstackable items (1 per slot)
        for(auto& slot : inventory) {
            if(remaining == 0) break;
            if (slot.id == ItemID::None) {
                slot.id = id;
                slot.quantity = 1;
                slot.qtyText.setString('1');
                remaining -= 1;
                slot.isFull = true;
            }
        }
    }
    isInventoryFull = isCompletelyFull();
    return amount - remaining;
}

void InventoryManager::reset() {

    for (auto& slot : inventory) {
        slot.id = ItemID::None;
        slot.quantity = 0;
        slot.isFull = false;
        slot.qtyText.setString('0');

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
    std::cout << "~~~~~ Inventory Start ~~~~~" << std::endl;
    for (const auto& slot : inventory) {
        std::cout << ItemDatabase::get(slot.id).name << ": " << slot.quantity << ", IsFull: " << slot.isFull << std::endl;
    }
}

// Check if inventory contains building cost items ; if exist, remove them from inventory
bool InventoryManager::checkItemsForBuilding(BuildingID id) {
    const BuildingData& data = BuildingDatabase::get(id);
    const auto& cost = data.buildCost;
    
    // Check if items in inventory
    for (const auto& entry : cost) {
        ItemID itemId = entry.id;
        int required = entry.amount;
        int available = getItemCount(itemId);

        if (available < required) {
            // return false if not enough items
            return false; 
        }
    }
    // If required items contained, remove them for construction
    for (const auto& entry : cost) {
        ItemID itemId = entry.id;
        int required  = entry.amount;
        removeItems(itemId, required);
    }

    return true;
}

// Check the number of items by ID
int InventoryManager::getItemCount(ItemID id) const {
    int itemCount = 0;
    for (const auto& slot : inventory) {
        if( id == slot.id ) itemCount += slot.quantity;
    }
    return itemCount;
}
// Remove items from inventory
void InventoryManager::removeItems(ItemID id, int amount) {
    if(id == ItemID::None || amount <= 0) return;

    for (auto& slot : inventory) {
        if( id == slot.id ) {
            if( slot.quantity < amount ) slot.quantity = 0;
            else slot.quantity -= amount;
        }
        slot.qtyText.setString(std::to_string(slot.quantity));
        slot.isFull = false;
    }
}

const std::vector<InventorySlot>& InventoryManager::getInventory() const {
    return inventory;
}

void InventoryManager::updateSlotTextures(float screenW, float screenH) {
    int i = 0;
    // initializing slot positions and sizes
    for (auto& slot : inventory) {
        slot.emptySlot.setSize({60.f, 60.f});
        slot.emptySlot.setOrigin({30.f, 30.f});
        slot.emptySlot.setFillColor(sf::Color(100, 100, 100, 150));
        
        sf::Vector2f spriteSize = slot.sprite.getGlobalBounds().size;
        float scaleX = 60.f / spriteSize.x;
        float scaleY = 60.f / spriteSize.y;
        slot.sprite.setScale({scaleX, scaleY});
        slot.sprite.setOrigin({spriteSize.x / 2.f, spriteSize.y/2.f});

        float row = (float)(i/5 + 1);
        float column = (float)(i%5 + 1);
        slot.emptySlot.setPosition({ screenW*column/7.f + 50.f, screenH*row/5.f + 50.f });
        slot.sprite.setPosition({ screenW*column/7.f + 50.f, screenH*row/5.f + 50.f });
        i++;

        slot.qtyText.setCharacterSize(12);
        slot.qtyText.setFillColor(sf::Color(255, 255, 255, 255));
        auto bounds = slot.qtyText.getLocalBounds();
        slot.qtyText.setOrigin({ bounds.size.x / 2, bounds.size.y / 2 });
        slot.qtyText.setPosition({ screenW*column/7.f + 50.f, screenH*row/5.f + 70.f } );
    }
}