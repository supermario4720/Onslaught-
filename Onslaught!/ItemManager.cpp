#include "ItemManager.hpp"
#include "Item.hpp"
#include "ItemData.hpp"
#include "InventoryManager.hpp"
#include <vector>
#include <iostream>


void ItemManager::spawn(ItemID id, const sf::Vector2f& pos, int qty) {
    const auto& data = ItemDatabase::get(id);

    std::shared_ptr<Item> newItem = std::make_shared<Item>(id, pos, qty);

    items.push_back(newItem);
}

void ItemManager::update(float dt, sf::Vector2f& playerPos, InventoryManager& playerInventory) {
    for (auto& item : items) {
        item -> update(dt, playerPos);
    }

    // remove expired items
    for (auto it = items.begin(); it != items.end(); ) {
        if ((*it)->isPickable()) {
            // ADD ITEM TO INVENTORY FIRST
            //
            int newQty = playerInventory.addItem((*it)->getItemID(), (*it)->getQuantity());
            (*it)->setQuantity(newQty);
            
            if ( (*it)->isExpired() ) {
                it = items.erase(it);
            }
        }
        else if ((*it)->isExpired()) {
            it = items.erase(it);
        }
        else {
            ++it;
        }
    }
}

void ItemManager::render(sf::RenderWindow& window) {
    for (const auto& item : items) {
        item -> render(window);
    }
}

void ItemManager::reset() {
    items.clear();
}
