#include "ItemManager.hpp"
#include "Item.hpp"
#include "Experience.hpp"
#include "ItemData.hpp"
#include "InventoryManager.hpp"
#include "PlayerStatusManager.hpp"
#include <vector>
#include <iostream>


void ItemManager::spawnItem(ItemID id, const sf::Vector2f& pos, int qty) {
    const auto& data = ItemDatabase::get(id);

    std::shared_ptr<Item> newItem = std::make_shared<Item>(id, pos, qty);

    items.push_back(newItem);
}

void ItemManager::spawnExp(const sf::Vector2f& pos, int qty) {
    std::shared_ptr<Experience> exp = std::make_shared<Experience>(pos, qty);

    experience.push_back(exp);
}

void ItemManager::update(float dt, sf::Vector2f& playerPos, InventoryManager& playerInventory, PlayerStatusManager& status) {
    // update items and experience
    for (auto& item : items) {
        item -> update(dt, playerPos);
    }
    for (auto& exp : experience) {
        exp -> update(dt, playerPos);
    }

    // remove expired items
    for (auto it = items.begin(); it != items.end(); ) {
        if ((*it)->isPickable()) {
            // add item to player inventory (returns quantity if not all could be added)
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

    // remove expired experience
    for (auto it = experience.begin(); it != experience.end(); ) {
        if ((*it)->isPickable()) {
            // add item to player inventory (returns quantity if not all could be added)
            status.addExperience((*it)->getQuantity());
            it = experience.erase(it);
        }
        else if ((*it)->isExpired()) {
            it = experience.erase(it);
        }
        else {
            ++it;
        }
    }
}

void ItemManager::render(sf::RenderWindow& window) {
    for (const auto& item : items) {
        item -> render(window);
    }for (const auto& exp : experience) {
        exp -> render(window);
    }
}

void ItemManager::reset() {
    items.clear();
    experience.clear();
}
